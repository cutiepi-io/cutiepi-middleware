### MCU protocol 

|               | Msg Type | MCU message    | Pi message |
|---------------+----------+----------------+------------|
| Button event  | 1 | Short click: 0x01     | ACK: 0xF1  |
|               | 1 | 3 seconds press: 0x02 (power on)  |  No ACK |
|               | 1 | 6 seconds press: 0x03 (power off) | Confirm: 0xF3, Cancel: 0xFC |
|               | 1 | 10 seconds press: force poweroff (no message) | No ACK | 
| Battery event | 2 | 2 bytes message, send every second, unit in mv |      |
| Charge event  | 3 | Charging: 0x4 | ACK: 0xF4 |
|               | 3 | Not charging: 0x5 | ACK: 0xF5 | 
| Version event | 4 |                       | 0xF6  |

### Payload format 

| header | msg type | length of payload | payload | crc | 
|--------+----------+-------------------+---------+-----| 
| 0xa55a | 1 byte   | 2 bytes           |         | sum of all other bytes | 
