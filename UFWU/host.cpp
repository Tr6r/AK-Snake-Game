#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>
#include <cstring>
#include "host.h"

using namespace std;

int fd = -1;
struct termios tty;
int flag = 0;
Uart_Boot_Send_State state = NO_SEND;
// Tính FCS giống MCU
uint8_t calc_fcs_pc(const uint8_t *data, uint8_t len)
{
	uint8_t fcs = len;
	for (size_t i = 0; i < len; i++)
		fcs ^= data[i];
	return fcs;
}
void uartInit(const char *uart_dev = "/dev/ttyUSB0")
{
	fd = open(uart_dev, O_RDWR | O_NOCTTY);
	if (fd < 0)
	{
		perror("open");
		return;
	}

	memset(&tty, 0, sizeof tty);
	if (tcgetattr(fd, &tty) != 0)
	{
		perror("tcgetattr");
		return;
	}

	cfsetospeed(&tty, B115200);
	cfsetispeed(&tty, B115200);

	tty.c_cflag |= (CLOCAL | CREAD);
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;
	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_oflag &= ~OPOST;

	tcsetattr(fd, TCSANOW, &tty);
}
void close_uart(int fd)
{
	tcflush(fd, TCIOFLUSH);
	tcdrain(fd);

	int flags = TIOCM_DTR | TIOCM_RTS;
	ioctl(fd, TIOCMBIC, &flags);

	close(fd);

	usleep(100000); // 100ms cho driver ổn định
}
void calculateFirmwareChecksum(const char *filename, firmware_header_t &header)
{
	FILE *f = fopen(filename, "rb");
	if (!f)
	{
		std::cerr << "Cannot open firmware file for checksum calculation\n";
		return;
	}

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	uint8_t *buf = new uint8_t[size];
	fread(buf, 1, size, f);

	fclose(f);

	// calculate checksum
	uint32_t sum = 0;
	for (size_t i = 0; i < size; i++)
		sum += buf[i];

	header.bin_len = size;
	header.checksum = sum & 0xFFFF;

	delete[] buf;
}

void readuart(uint8_t ch)
{

	if (ch == '\r' || ch == '\n')
	{
		std::cout << std::endl; // xuống hàng
	}
	else if (ch >= 32 && ch <= 126) // ký tự printable
	{
		std::cout << (char)ch; // in liền
	}
	else
	{
		std::cout << "0x" << std::hex << (int)ch << std::dec; // in hex liền
	}
}
bool waitForAck(int fd)
{
	uint8_t ch;

	while (true)
	{
		int n = read(fd, &ch, 1);

		if (n > 0)
		{

			if (ch == UART_BOOT_FWU_ACK) // ACK
			{
				return true;
			}
			else if (ch == UART_BOOT_FWU_READY) // READY
			{
				return true;
			}
			else if (ch == UART_BOOT_FWU_NACK) // NACK
			{
				return false;
			}
			readuart(ch);
		}
	}
}

void printProgress(size_t current, size_t total)
{
	const int barWidth = 50; // độ dài thanh progress
	float ratio = (float)current / total;
	int pos = ratio * barWidth;

	std::cout << "[";
	for (int i = 0; i < barWidth; i++)
	{
		if (i < pos)
			std::cout << "=";
		else if (i == pos)
			std::cout << ">";
		else
			std::cout << " ";
	}
	std::cout << "] " << int(ratio * 100.0) << " %\r";
	std::cout.flush();
}
bool sendByteAndWaitACK(uint8_t byte)
{

	for (int attempt = 1; attempt <= MAX_RETRY; attempt++)
	{
		usleep(3000);
		write(fd, &byte, 1);
		tcdrain(fd);

		uint8_t ch;
		int dot_count = 0;
		if (waitForAck(fd))
		{
			return true;
		}
		if (state == SEND_FCS_STATE)
		{
			return false;
		}
		std::cout << "Retrying to send byte (attempt " << attempt << ")...\n";
	}

	std::cout << "ERROR: byte failed too many retries!\n";
	return false;
}

bool sendFrameWithACK(const uint8_t *data, uint8_t len, uint8_t sop = 0xEF)
{
	// 1. SOP
	// cout << "[host]SEND SOP" << endl;
	if (!sendByteAndWaitACK(sop))
		return false;
	// 2. LEN
	cout << "[host]SEND LEN: " << (int)len << endl;
	if (!sendByteAndWaitACK(len))
		return false;

	// 3. DATA
	// cout << "[host]SEND DATA" << endl;
	for (uint8_t i = 0; i < len; i++)
	{

		write(fd, &data[i], 1);

		usleep(10);
	}

	if (!waitForAck(fd))
	{
		return false;
	}

	// 4. FCS
	// uint8_t fcs = calc_fcs_pc(data, len);
	uint8_t fcs = calc_fcs_pc(data, len);
	// cout << "[host]CALC FCS: " << (int)fcs << endl;

	if (!sendByteAndWaitACK(fcs))
		return false;
	// cout << "[host]Rx ACK: " << (int)fcs << endl;
	return true;
}

bool sendFrameRetry(const uint8_t *data, uint16_t len)
{
	const int MAX_FRAME_RETRY = 3;

	for (int attempt = 1; attempt <= MAX_FRAME_RETRY; attempt++)
	{
		std::cout << "FRAME TRY " << attempt << "/" << MAX_FRAME_RETRY << "\n";

		if (sendFrameWithACK(data, len))
			return true;

		std::cout << "Frame failed, retrying...\n";
	}

	std::cout << "FRAME FAILED after retries!\n";
	return false;
}
bool sendReadySignal()
{
	cout << "[host]SEND UPDATE FW CMD" << endl;
	write(fd, "fwset\r\n", 7);
	uint8_t ch;
	cout << "WAITING READY ACK" << endl;

	if (!waitForAck(fd))
	{
		return false;
	}
	return true;
}
int main()
{

	uartInit("/dev/ttyUSB0");
	// SEND UPDATE FW CMD
	for (int i = 0; i < 3; i++)
	{
		if (sendReadySignal())
		{
			cout << "[host]RX READY ACK!" << endl;

			break;
		}
		cout << "RESEND UPDATE FW CMD: " << i + 1 << endl;
	}

	// const char *filename = "snake.bin";
	const char *filename ="ak.bin";
	// Gửi update request với metadata
	firmware_header_t meta = {};
	calculateFirmwareChecksum(filename, meta);

	meta.psk = FIRMWARE_PSK;
	sendFrameWithACK((uint8_t *)&meta, sizeof(meta));

	FILE *fw = fopen(filename, "rb");
	if (!fw)
	{
		printf("Cannot open firmware file\n");
		return -1;
	}
	uint8_t buffer[CHUNK_SIZE];
	size_t bytes_read;
	size_t total_size = meta.bin_len;
	size_t total_chunks = (total_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
	size_t chunk_index = 0;
	uint32_t offset = 0;
	while (offset < meta.bin_len)
	{
		uint32_t remaining = meta.bin_len - offset;
		uint32_t chunk = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : remaining;
		// đọc đúng chunk
		size_t bytes_read = fread(buffer, 1, chunk, fw);
		if (bytes_read != chunk)
		{
			std::cout << "READ ERROR: expected " << chunk << ", got " << bytes_read << endl;
			return -1;
		}

		if (!sendFrameRetry((uint8_t *)&buffer, chunk))
		{
			std::cout << "UPLOAD FAILED!" << endl;
			return -1;
		}
		offset += chunk;
		chunk_index++;
		std::cout << "chunk_index: " << chunk_index << endl;
		// printProgress(chunk_index, total_chunks);
	}
	std::cout << "[BOOT]SEND ALL DATA!" << endl;

	waitForAck(fd);
	close_uart(fd);

	return 0;
}
