> 查看是否有符号表

```bash
sh-4.4# readelf -S /usr/bin/TestServer | grep debug
  [29] .debug_aranges    PROGBITS         0000000000000000  00303480
  [30] .debug_info       PROGBITS         0000000000000000  0031a090
  [31] .debug_abbrev     PROGBITS         0000000000000000  01dbc609
  [32] .debug_line       PROGBITS         0000000000000000  01e0f9b6
  [33] .debug_str        PROGBITS         0000000000000000  01f97c7b
  [34] .debug_loc        PROGBITS         0000000000000000  027fc876
  [35] .debug_ranges     PROGBITS         0000000000000000  03741000
```