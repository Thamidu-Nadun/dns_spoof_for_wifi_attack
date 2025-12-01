# Release.md

## Instruction

> [!TIP]
> If you have not dealt with a microcontroller, please follow the instructions in [README](../README.md).

You should have esptool to perform this task.

```bash
esptool.exe --chip esp32 write_flash 0x0 ./dns_spoof_v1.bin
```

> [!TIP]
> You can use other flags if you want, such as `--port`, `--before default_reset`, etc.

> [!NOTE]
> This released binary is a merged one of bootloader, partition-table, app and firmware binaries.
> And by default it compiled with [Dialog_template](../template/dialog.html).

---
