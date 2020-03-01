#include "qmodbus.h"


#define EMBBADCRC  1
#define EMBBADDATA  2
#define EMBBADEXC   3
#define EMBUNKEXC   4
#define EMBMDATA    5

#define MSG_LENGTH_UNDEFINED -1

#define NOME_PORTA "/dev/ttyS0"

size_t strlcpy(char *dest, const char *src, size_t dest_size)
{
    register char *d = dest;
    register const char *s = src;
    register size_t n = dest_size;

    /* Copy as many bytes as will fit */
    if (n != 0 && --n != 0) {
        do {
            if ((*d++ = *s++) == 0)
                break;
        } while (--n != 0);
    }

    /* Not enough room in dest, add NUL and traverse rest of src */
    if (n == 0) {
        if (dest_size != 0)
            *d = '\0'; /* NUL-terminate dest */
        while (*s++)
            ;
    }

    return (s - src - 1); /* count does not include NUL */
}

QModbus::QModbus()
{
    size_t dest_size;
    size_t ret_size;

    dest_size = sizeof(device);
    ret_size = strlcpy(device, NOME_PORTA, dest_size);

    slave=-1;

    backend_type=_MODBUS_BACKEND_TYPE_RTU;
    header_length=_MODBUS_RTU_HEADER_LENGTH;
    checksum_length=_MODBUS_RTU_CHECKSUM_LENGTH;
    max_adu_length=_MODBUS_RTU_MAX_ADU_LENGTH;

}


int QModbus::set_slave( int newslave)
{

    if (newslave >= 0 && newslave <= 247) {
        slave = newslave;
    } else {
        errno = EINVAL;
        return -1;
    }

    return 0;
}


int QModbus::write_bits(int addr, int nb, const uint8_t *src)
{
    int rc;
    int i;
    int byte_count;
    int req_length;
    uint8_t req[MAX_MESSAGE_LENGTH];

    if (nb > MODBUS_MAX_WRITE_BITS) {

        return -1;
    }

    req_length = build_request_basis(_FC_WRITE_MULTIPLE_COILS,addr, nb, req);
    byte_count = (nb / 8) + ((nb % 8) ? 1 : 0);
    req[req_length++] = byte_count;

    for (i = 0; i < byte_count; i++) {
        req[req_length] = src[i];
        req_length++;
    }

    //QTime time;
    //time.start();
    rc = send( req, req_length);
    if (rc > 0) {
        //qDebug() << "TEMPO SPEDIZIONE" << time.elapsed();
        //time.start();
        uint8_t rsp[MAX_MESSAGE_LENGTH];
        rc = receive_msg_req( req, rsp);
        //rc = recv( req, rsp);
        //qDebug() << "TEMPO RICEZIONE" << time.elapsed();
    }


    return rc;

}

unsigned int QModbus::compute_response_length_from_request( uint8_t *req)
{
    int length;
    int offset;

    offset = header_length;

    switch (req[offset]) {
    case _FC_READ_COILS:
    case _FC_READ_DISCRETE_INPUTS: {
        /* Header + nb values (code from write_bits) */
        int nb = (req[offset + 3] << 8) | req[offset + 4];
        length = 2 + (nb / 8) + ((nb % 8) ? 1 : 0);
    }
        break;
    case _FC_READ_AND_WRITE_REGISTERS:
    case _FC_READ_HOLDING_REGISTERS:
    case _FC_READ_INPUT_REGISTERS:
        /* Header + 2 * nb values */
        length = 2 + 2 * (req[offset + 3] << 8 | req[offset + 4]);
        break;
    case _FC_READ_EXCEPTION_STATUS:
        length = 3;
        break;
    case _FC_REPORT_SLAVE_ID:
        /* The response is device specific (the header provides the
           length) */
        return MSG_LENGTH_UNDEFINED;
    default:
        length = 5;
    }

    return offset + length + checksum_length;
}

uint8_t QModbus::compute_meta_length_after_function(int function,msg_type_t msg_type)
{
    int length;

    if (msg_type == MSG_INDICATION) {
        if (function <= _FC_WRITE_SINGLE_REGISTER) {
            length = 4;
        } else if (function == _FC_WRITE_MULTIPLE_COILS ||
                   function == _FC_WRITE_MULTIPLE_REGISTERS) {
            length = 5;
        } else if (function == _FC_READ_AND_WRITE_REGISTERS) {
            length = 9;
        } else {
            /* _FC_READ_EXCEPTION_STATUS, _FC_REPORT_SLAVE_ID */
            length = 0;
        }
    } else {
        /* MSG_CONFIRMATION */
        switch (function) {
        case _FC_WRITE_SINGLE_COIL:
        case _FC_WRITE_SINGLE_REGISTER:
        case _FC_WRITE_MULTIPLE_COILS:
        case _FC_WRITE_MULTIPLE_REGISTERS:
            length = 4;
            break;
        default:
            length = 1;
        }
    }

    return length;
}

/* Computes the length to read after the meta information (address, count, etc) */
int QModbus::compute_data_length_after_meta( uint8_t *msg, msg_type_t msg_type)
{
    int function = msg[header_length];
    int length;

    if (msg_type == MSG_INDICATION) {
        switch (function) {
        case _FC_WRITE_MULTIPLE_COILS:
        case _FC_WRITE_MULTIPLE_REGISTERS:
            length = msg[header_length + 5];
            break;
        case _FC_READ_AND_WRITE_REGISTERS:
            length = msg[header_length + 9];
            break;
        default:
            length = 0;
        }
    } else {
        /* MSG_CONFIRMATION */
        if (function <= _FC_READ_INPUT_REGISTERS ||
            function == _FC_REPORT_SLAVE_ID ||
            function == _FC_READ_AND_WRITE_REGISTERS) {
            length = msg[header_length + 1];
        } else {
            length = 0;
        }
    }

    length += checksum_length;

    return length;
}


int QModbus::receive_msg_req( uint8_t *req, uint8_t *rsp)
{
    int rc;
    int rsp_length_computed;
    int offset =header_length;


    rc = receive_msg( rsp, MSG_CONFIRMATION);
    if (rc == -1) {
        return -1;
    }

    rsp_length_computed = compute_response_length_from_request( req);

    /* Check length */
    if (rc == rsp_length_computed ||
        rsp_length_computed == MSG_LENGTH_UNDEFINED) {
        int req_nb_value;
        int rsp_nb_value;
        int function = rsp[offset];
        int num_items = 1;
        int addr = (req[offset + 1] << 8) + req[offset + 2];

        /* Check function code */
        if (function != req[offset]) {
            if (debug) {
                fprintf(stderr,
                        "Received function not corresponding to the request (%d != %d)\n",
                        function, req[offset]);
            }
            errno = EMBBADDATA;
            return -1;
        }

        /* Check the number of values is corresponding to the request */
        switch (function) {
        case _FC_READ_COILS:
        case _FC_READ_DISCRETE_INPUTS:
            /* Read functions, 8 values in a byte (nb
             * of values in the request and byte count in
             * the response. */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            req_nb_value = (req_nb_value / 8) + ((req_nb_value % 8) ? 1 : 0);
            rsp_nb_value = rsp[offset + 1];
            break;
        case _FC_READ_AND_WRITE_REGISTERS:
        case _FC_READ_HOLDING_REGISTERS:
        case _FC_READ_INPUT_REGISTERS:
            /* Read functions 1 value = 2 bytes */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            rsp_nb_value = (rsp[offset + 1] / 2);
            break;
        case _FC_WRITE_MULTIPLE_COILS:
        case _FC_WRITE_MULTIPLE_REGISTERS:
            /* N Write functions */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            rsp_nb_value = (rsp[offset + 3] << 8) | rsp[offset + 4];
            break;
        case _FC_REPORT_SLAVE_ID:
            /* Report slave ID (bytes received) */
            req_nb_value = rsp_nb_value = rsp[offset + 1];
            break;
        default:
            /* 1 Write functions & others */
            req_nb_value = rsp_nb_value = 1;
        }

        num_items = rsp_nb_value;
        switch (function)
        {
            case _FC_READ_COILS:
            case _FC_READ_DISCRETE_INPUTS:
                num_items = rsp_nb_value*8;
                break;
            case _FC_READ_AND_WRITE_REGISTERS:
            case _FC_READ_HOLDING_REGISTERS:
            case _FC_READ_INPUT_REGISTERS:
                num_items = rsp_nb_value/2;
                break;
            case _FC_WRITE_MULTIPLE_COILS:
            case _FC_WRITE_MULTIPLE_REGISTERS:
                addr = (rsp[offset + 1] << 8) | rsp[offset + 2];
                num_items = rsp_nb_value;
                break;
            default:
                break;
        }


        if (req_nb_value == rsp_nb_value) {
            rc = rsp_nb_value;
        } else {
            if (debug) {
                fprintf(stderr,
                        "Quantity not corresponding to the request (%d != %d)\n",
                        rsp_nb_value, req_nb_value);
            }
            errno = EMBBADDATA;
            rc = -1;
        }
    } else if (rc == (int)(offset + 2 + checksum_length) &&
               req[offset] == (rsp[offset] - 0x80)) {
        /* EXCEPTION CODE RECEIVED */

        int exception_code = rsp[offset + 1];
        //if (exception_code < MODBUS_EXCEPTION_MAX) {
        //    ;//errno = MODBUS_ENOBASE + exception_code;
        //} else {
        //    errno = EMBBADEXC;
        //}
        //_error_print(ctx, NULL);
        rc = -1;
    } else {
        if (debug) {
            fprintf(stderr,
                    "Message length not corresponding to the computed length (%d != %d)\n",
                    rc, rsp_length_computed);
        }
        errno = EMBBADDATA;
        rc = -1;
    }

    return rc;
}


int QModbus::receive_msg( uint8_t *msg, msg_type_t msg_type)
{
    int rc;
    fd_set rfds;
    struct timeval tv;
    int length_to_read;
    uint8_t *p_msg;
    int msg_length = 0;
    _step_t step;

    if (debug) {
        if (msg_type == MSG_INDICATION) {
            printf("Waiting for a indication...\n");
        } else {
            printf("Waiting for a confirmation...\n");
        }
    }

    /* Add a file descriptor to the set */
    FD_ZERO(&rfds);
    FD_SET(Fd, &rfds);

    /* We need to analyse the message step by step.  At the first step, we want
     * to reach the function code because all packets contain this
     * information. */
    step = _STEP_FUNCTION;
    length_to_read = header_length + 1;

    ///MODIFICO////
    //usleep(10000);
    tv.tv_sec = 0;
    tv.tv_usec = 500000;

    p_msg = msg;
    while (length_to_read != 0) {
        /*rc = Select( &rfds, &tv, length_to_read);
        if (rc == -1) {
            return -1;
        }*/

        rc = recv( p_msg, length_to_read);
        if (rc == 0) {
            errno = ECONNRESET;
            rc = -1;
        }

        if (rc == -1) {
            //_error_print(ctx, "read");
            //if (ctx->error_recovery && (errno == ECONNRESET ||
            //                            errno == ECONNREFUSED)) {
                chiudi();
                connect();
                /* Could be removed by previous calls */
                errno = ECONNRESET;
            //}
            return -1;
        }


        /* Display the hex code of each character received */
        if (debug) {
            int i;
            for (i=0; i < rc; i++)
                printf("<%.2X>", p_msg[i]);
        }

        /* Moves the pointer to receive other data */
        p_msg = &(p_msg[rc]);
        /* Sums bytes received */
        msg_length += rc;
        /* Computes remaining bytes */
        length_to_read -= rc;

        if (length_to_read == 0) {
            switch (step) {
            case _STEP_FUNCTION:
                /* Function code position */
                length_to_read =  compute_meta_length_after_function(
                   msg[header_length],
                   msg_type);
                if (length_to_read != 0) {
                    step = _STEP_META;
                    break;
                } /* else switches straight to the next step */
            case _STEP_META:
                length_to_read = compute_data_length_after_meta( msg, msg_type);
                if ((msg_length + length_to_read) > (int)max_adu_length) {
                    errno = EMBBADDATA;
                    //_error_print(ctx, "too many data");
                    return -1;
                }
                step = _STEP_DATA;
                break;
            default:
                break;
            }
        }

        if (length_to_read > 0) {
            /* If no character at the buffer wait
               TIME_OUT_END_OF_TRAME before raising an error. */
            tv.tv_sec =0;
            tv.tv_usec = 1000;
        }
    }

    if (debug)
        printf("\n");

    return check_integrity( msg, msg_length);
}

int QModbus::Select( fd_set *rfds, struct timeval *tv, int length_to_read)
{
    int s_rc;

    while ((s_rc = select(Fd+1, rfds, NULL, NULL, tv)) == -1) {
        if (errno == EINTR) {
            if (debug) {
                fprintf(stderr, "A non blocked signal was caught\n");
            }
            /* Necessary after an error */
            FD_ZERO(rfds);
            FD_SET(Fd, rfds);
        } else {
            /*_error_print(ctx, "select");
            if (ctx->error_recovery && (errno == EBADF)) {
                modbus_close(ctx);
                modbus_connect(ctx);
                errno = EBADF;
                return -1;
            } else {
                return -1;
            }*/
            return -1;
        }
    }

    if (s_rc == 0) {
        /* Timeout */
        errno = ETIMEDOUT;
        //_error_print(ctx, "select");
        return -1;
    }


    return s_rc;
}


//#include "modbus-private.h"

//#include "modbus-rtu.h"
//#include "modbus-rtu-private.h"

/* Table of CRC values for high-order byte */
static const uint8_t table_crc_hi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
static const uint8_t table_crc_lo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};



/* Builds a RTU request header */
int QModbus::build_request_basis( int function,int addr, int nb, uint8_t *req)
{
    assert(slave != -1);
    req[0] = slave;
    req[1] = function;
    req[2] = addr >> 8;
    req[3] = addr & 0x00ff;
    req[4] = nb >> 8;
    req[5] = nb & 0x00ff;

    return _MODBUS_RTU_PRESET_REQ_LENGTH;
}

/* Builds a RTU response header */
/*int QModbus::build_response_basis(sft_t *sft, uint8_t *rsp)
{

    rsp[0] = sft->slave;
    rsp[1] = sft->function;

    return _MODBUS_RTU_PRESET_REQ_LENGTH;;
}*/

uint16_t QModbus::crc16(uint8_t *buffer, uint16_t buffer_length)
{
    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--) {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}

/*int QModbus::prepare_response_tid(const uint8_t *req, int *req_length)
{
    (*req_length) -= CHECKSUM_LENGTH;

    return 0;
}*/

int QModbus::send_msg_pre(uint8_t *req, int req_length)
{
    uint16_t crc = crc16(req, req_length);
    req[req_length++] = crc >> 8;
    req[req_length++] = crc & 0x00FF;

    return req_length;
}



ssize_t QModbus::send( const uint8_t *req, int req_length)
{

    return write(Fd, req, req_length);

}

ssize_t QModbus::recv( uint8_t *rsp, int rsp_length)
{

    return read(Fd, rsp, rsp_length);

}

//int QModbus::flush(modbus_t *);

/* The check_crc16 function shall return the message length if the CRC is
   valid. Otherwise it shall return -1 and set errno to EMBADCRC. */
int QModbus::check_integrity( uint8_t *msg,const int msg_length)
{
    uint16_t crc_calculated;
    uint16_t crc_received;

    crc_calculated = crc16(msg, msg_length - 2);
    crc_received = (msg[msg_length - 2] << 8) | msg[msg_length - 1];

    last_crc_expected = crc_calculated;
    last_crc_received = crc_received;

    /* Check CRC of msg */
    if (crc_calculated == crc_received) {
        return msg_length;
    } else {
        return -1;
    }
}

/* Sets up a serial port for RTU communications */
int QModbus::connect()
{

    struct termios tios;
    speed_t speed;

    baud=38400;
    //device="/dev/ttyS0" ;
    data_bit=8;
    parity = 'N';
    stop_bit = 1;


    /* The O_NOCTTY flag tells UNIX that this program doesn't want
       to be the "controlling terminal" for that port. If you
       don't specify this then any input (such as keyboard abort
       signals and so forth) will affect your process

       Timeouts are ignored in canonical input mode or when the
       NDELAY option is set on the file via open or fcntl */
    Fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_EXCL);
    if (Fd == -1) {
        fprintf(stderr, "ERROR Can't open the device %s (%s)\n",
                NOME_PORTA, strerror(errno));
        return -1;
    }

    /* Save */
    tcgetattr(Fd, &(old_tios));

    memset(&tios, 0, sizeof(struct termios));

    /* C_ISPEED     Input baud (new interface)
       C_OSPEED     Output baud (new interface)
    */
    switch (baud) {
    case 110:
        speed = B110;
        break;
    case 300:
        speed = B300;
        break;
    case 600:
        speed = B600;
        break;
    case 1200:
        speed = B1200;
        break;
    case 2400:
        speed = B2400;
        break;
    case 4800:
        speed = B4800;
        break;
    case 9600:
        speed = B9600;
        break;
    case 19200:
        speed = B19200;
        break;
    case 38400:
        speed = B38400;
        break;
    case 57600:
        speed = B57600;
        break;
    case 115200:
        speed = B115200;
        break;
    default:
        speed = B9600;

    }

    /* Set the baud rate */
    if ((cfsetispeed(&tios, speed) < 0) ||
        (cfsetospeed(&tios, speed) < 0)) {
        return -1;
    }

    /* C_CFLAG      Control options
       CLOCAL       Local line - do not change "owner" of port
       CREAD        Enable receiver
    */
    tios.c_cflag |= (CREAD | CLOCAL);
    /* CSIZE, HUPCL, CRTSCTS (hardware flow control) */

    /* Set data bits (5, 6, 7, 8 bits)
       CSIZE        Bit mask for data bits
    */
    tios.c_cflag &= ~CSIZE;
    switch (data_bit) {
    case 5:
        tios.c_cflag |= CS5;
        break;
    case 6:
        tios.c_cflag |= CS6;
        break;
    case 7:
        tios.c_cflag |= CS7;
        break;
    case 8:
    default:
        tios.c_cflag |= CS8;
        break;
    }

    /* Stop bit (1 or 2) */
    if (stop_bit == 1)
        tios.c_cflag &=~ CSTOPB;
    else /* 2 */
        tios.c_cflag |= CSTOPB;

    /* PARENB       Enable parity bit
       PARODD       Use odd parity instead of even */
    if (parity == 'N') {
        /* None */
        tios.c_cflag &=~ PARENB;
    } else if (parity == 'E') {
        /* Even */
        tios.c_cflag |= PARENB;
        tios.c_cflag &=~ PARODD;
    } else {
        /* Odd */
        tios.c_cflag |= PARENB;
        tios.c_cflag |= PARODD;
    }

    /* Read the man page of termios if you need more information. */

    /* This field isn't used on POSIX systems
       tios.c_line = 0;
    */

    /* C_LFLAG      Line options

       ISIG Enable SIGINTR, SIGSUSP, SIGDSUSP, and SIGQUIT signals
       ICANON       Enable canonical input (else raw)
       XCASE        Map uppercase \lowercase (obsolete)
       ECHO Enable echoing of input characters
       ECHOE        Echo erase character as BS-SP-BS
       ECHOK        Echo NL after kill character
       ECHONL       Echo NL
       NOFLSH       Disable flushing of input buffers after
       interrupt or quit characters
       IEXTEN       Enable extended functions
       ECHOCTL      Echo control characters as ^char and delete as ~?
       ECHOPRT      Echo erased character as character erased
       ECHOKE       BS-SP-BS entire line on line kill
       FLUSHO       Output being flushed
       PENDIN       Retype pending input at next read or input char
       TOSTOP       Send SIGTTOU for background output

       Canonical input is line-oriented. Input characters are put
       into a buffer which can be edited interactively by the user
       until a CR (carriage return) or LF (line feed) character is
       received.

       Raw input is unprocessed. Input characters are passed
       through exactly as they are received, when they are
       received. Generally you'll deselect the ICANON, ECHO,
       ECHOE, and ISIG options when using raw input
    */

    /* Raw input */
    tios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /* C_IFLAG      Input options

       Constant     Description
       INPCK        Enable parity check
       IGNPAR       Ignore parity errors
       PARMRK       Mark parity errors
       ISTRIP       Strip parity bits
       IXON Enable software flow control (outgoing)
       IXOFF        Enable software flow control (incoming)
       IXANY        Allow any character to start flow again
       IGNBRK       Ignore break condition
       BRKINT       Send a SIGINT when a break condition is detected
       INLCR        Map NL to CR
       IGNCR        Ignore CR
       ICRNL        Map CR to NL
       IUCLC        Map uppercase to lowercase
       IMAXBEL      Echo BEL on input line too long
    */
    if (parity == 'N') {
        /* None */
        tios.c_iflag &= ~INPCK;
    } else {
        tios.c_iflag |= INPCK;
    }

    /* Software flow control is disabled */
    tios.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* C_OFLAG      Output options
       OPOST        Postprocess output (not set = raw output)
       ONLCR        Map NL to CR-NL

       ONCLR ant others needs OPOST to be enabled
    */

    /* Raw ouput */
    tios.c_oflag &=~ OPOST;

    /* C_CC         Control characters
       VMIN         Minimum number of characters to read
       VTIME        Time to wait for data (tenths of seconds)

       UNIX serial interface drivers provide the ability to
       specify character and packet timeouts. Two elements of the
       c_cc array are used for timeouts: VMIN and VTIME. Timeouts
       are ignored in canonical input mode or when the NDELAY
       option is set on the file via open or fcntl.

       VMIN specifies the minimum number of characters to read. If
       it is set to 0, then the VTIME value specifies the time to
       wait for every character read. Note that this does not mean
       that a read call for N bytes will wait for N characters to
       come in. Rather, the timeout will apply to the first
       character and the read call will return the number of
       characters immediately available (up to the number you
       request).

       If VMIN is non-zero, VTIME specifies the time to wait for
       the first character read. If a character is read within the
       time given, any read will block (wait) until all VMIN
       characters are read. That is, once the first character is
       read, the serial interface driver expects to receive an
       entire packet of characters (VMIN bytes total). If no
       character is read within the time allowed, then the call to
       read returns 0. This method allows you to tell the serial
       driver you need exactly N bytes and any read call will
       return 0 or N bytes. However, the timeout only applies to
       the first character read, so if for some reason the driver
       misses one character inside the N byte packet then the read
       call could block forever waiting for additional input
       characters.

       VTIME specifies the amount of time to wait for incoming
       characters in tenths of seconds. If VTIME is set to 0 (the
       default), reads will block (wait) indefinitely unless the
       NDELAY option is set on the port with open or fcntl.
    */
    /* Unused because we use open with the NDELAY option */
    tios.c_cc[VMIN] = 0;
    tios.c_cc[VTIME] = 0;

    if (tcsetattr(Fd, TCSANOW, &tios) < 0) {
        return -1;
    }


    return 0;
}

void QModbus::chiudi()
{

    tcsetattr(Fd, TCSANOW, &(old_tios));
    close(Fd);

}

int QModbus::flush()
{

    return tcflush(Fd, TCIOFLUSH);

}





