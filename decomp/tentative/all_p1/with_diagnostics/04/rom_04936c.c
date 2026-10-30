/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04936c.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x000493e5) */

void candidate_4936c(void)

{
  undefined4 extraout_A0;
  int unaff_A6;
  undefined1 uVar1;

  if (0x13 < *(ushort *)(unaff_A6 + 0x6e)) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  *(ushort *)(unaff_A6 + 0x7a) = *(ushort *)(unaff_A6 + 0x6e) * 0x10 + 0x100;
  uVar1 = false;
  if (*(char *)(unaff_A6 + 0x9b) == '\0') {
    candidate_1aeda();
    if (!(bool)uVar1) {
      candidate_1af9a();
      *(undefined4 *)(unaff_A6 + 0x7c) = extraout_A0;
      uVar1 = 0;
    }
    candidate_49cb2();
    if ((bool)uVar1) {
      *(undefined2 *)(unaff_A6 + 0x78) = *(undefined2 *)(*(int *)(unaff_A6 + 0x7c) + 0x3c);
    }
    if (*(short *)(unaff_A6 + 0x3c) < 0x18) {
      *(undefined2 *)(unaff_A6 + 0x78) = *(undefined2 *)(*(int *)(unaff_A6 + 0x7c) + 0x3c);
      *(undefined2 *)(unaff_A6 + 0x80) = 0xc0;
      *(undefined1 *)(unaff_A6 + 0x9b) = 0xff;
    }
    if (0x128 < *(short *)(unaff_A6 + 0x3c)) {
      *(undefined2 *)(unaff_A6 + 0x78) = *(undefined2 *)(*(int *)(unaff_A6 + 0x7c) + 0x3c);
      *(undefined2 *)(unaff_A6 + 0x80) = 0xc0;
      *(undefined1 *)(unaff_A6 + 0x9b) = 0xff;
    }
  }
  return;
}
