### MCU protocol 

|               | MCU message           | Pi message |
|---------------|-----------------------|------------|
| Button event  | Short click: 0x01     | ACK: 0xF1  |
|               | 3 seconds click: 0x02 (power on) |  No ACK |
|               | 6 seconds press: 0x03 | Confirm: 0xF3, Cancel: 0xFC |
|               | 10 seconds press: force poweroff (no message) | No ACK | 
| Battery event | 2 bytes message, send every second, unit in mv |      |
| Charging event| Charging: 0x4 | ACK: 0xF4 |
|               | Not charging: 0x5 | ACK: 0xF5 | 

### Payload format 

| header | length of payload | payload | crc | 
|--------|-------------------|---------|-----| 
| 0xa55a | 2 bytes | | sum of all other bytes| 
