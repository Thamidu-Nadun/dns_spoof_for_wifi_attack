# Release.md

## Instruction

> [!TIP]
> If you have not deal with micro-controller please follow instruction in (../README.md)[README.MD].

you should have esptool to do this.

```bash
esptool.exe --chip esp32 write_flash 0x0 ./dns_spoof_v1.bin
```

> [!TIP]
> You can use other flags if you want such as `--port`, `--before default_reset` etc.

> [!INFO]
> This released binary is a merged one of bootloader, partition-table, app and firmware binaries.
> And by default it compiled with (dialog_template)[../template/dialog.html].

---
