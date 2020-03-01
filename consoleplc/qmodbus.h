#ifndef QMODBUS_H
#define QMODBUS_H


#include <stdint.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>


/*enum {
    MODBUS_EXCEPTION_ILLEGAL_FUNCTION = 0x01,
    MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
    MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
    MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE,
    MODBUS_EXCEPTION_ACKNOWLEDGE,
    MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
    MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE,
    MODBUS_EXCEPTION_MEMORY_PARITY,
    MODBUS_EXCEPTION_NOT_DEFINED,
    MODBUS_EXCEPTION_GATEWAY_PATH,
    MODBUS_EXCEPTION_GATEWAY_TARGET,
    MODBUS_EXCEPTION_MAX
};*/



#define _MODBUS_BACKEND_TYPE_RTU       0
#define _MODBUS_RTU_MAX_ADU_LENGTH     256
#define _MODBUS_RTU_HEADER_LENGTH      1
#define _MODBUS_RTU_PRESET_REQ_LENGTH  6
#define _MODBUS_RTU_PRESET_RSP_LENGTH  2
#define _MODBUS_RTU_CHECKSUM_LENGTH    2

/* Function codes */
#define _FC_READ_COILS                0x01
#define _FC_READ_DISCRETE_INPUTS      0x02
#define _FC_READ_HOLDING_REGISTERS    0x03
#define _FC_READ_INPUT_REGISTERS      0x04
#define _FC_WRITE_SINGLE_COIL         0x05
#define _FC_WRITE_SINGLE_REGISTER     0x06
#define _FC_READ_EXCEPTION_STATUS     0x07
#define _FC_WRITE_MULTIPLE_COILS      0x0F
#define _FC_WRITE_MULTIPLE_REGISTERS  0x10
#define _FC_REPORT_SLAVE_ID           0x11
#define _FC_READ_AND_WRITE_REGISTERS  0x17

#define MAX_MESSAGE_LENGTH 260

#define MODBUS_MAX_READ_REGISTERS          125
#define MODBUS_MAX_WRITE_REGISTERS         123
#define MODBUS_MAX_RW_WRITE_REGISTERS      121

#define MODBUS_MAX_READ_BITS              2000
#define MODBUS_MAX_WRITE_BITS             1968

typedef enum {
    /* Request message on the server side */
    MSG_INDICATION,
    /* Request message on the client side */
    MSG_CONFIRMATION
} msg_type_t;

typedef enum {
    _STEP_FUNCTION,
    _STEP_META,
    _STEP_DATA
} _step_t;

class QModbus
{
public:
    QModbus();
    int set_slave( int newslave);
    int write_bits(int addr, int nb, const uint8_t *src);
    int connect();
    void chiudi();


private:
    int receive_msg_req( uint8_t *req, uint8_t *rsp);
    int receive_msg( uint8_t *msg, msg_type_t msg_type);

private:
    int build_request_basis( int function,int addr, int nb, uint8_t *req);
    uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);
    int send_msg_pre(uint8_t *req, int req_length);
    ssize_t send( const uint8_t *req, int req_length);
    ssize_t recv( uint8_t *rsp, int rsp_length);
    int check_integrity( uint8_t *msg,const int msg_length);
    unsigned int compute_response_length_from_request( uint8_t *req);
    uint8_t compute_meta_length_after_function(int function,msg_type_t msg_type);
    int compute_data_length_after_meta( uint8_t *msg, msg_type_t msg_type);
    int flush();
    int Select( fd_set *rfds, struct timeval *tv, int length_to_read);

private:
    int Fd;
    char device[16];
    int baud;
    uint8_t data_bit;
    uint8_t stop_bit;
    char parity;
    struct termios old_tios;
    int slave;
    uint16_t last_crc_expected;
    uint16_t last_crc_received;
    unsigned int backend_type;
    unsigned int header_length;
    unsigned int checksum_length;
    unsigned int max_adu_length;
    bool debug;
};

#endif // QMODBUS_H
