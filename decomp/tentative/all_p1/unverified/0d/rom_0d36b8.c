/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d36b8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d36b8(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  int unaff_A6;
  bool bVar2;

  *(undefined1 *)(unaff_A6 + 0x73) = 10;
  do {
    *(char *)(unaff_A6 + 0x73) = *(char *)(unaff_A6 + 0x73) + -1;
    bVar2 = false;
    candidate_40bc();
    iVar1 = extraout_A0;
    if (bVar2) {
      candidate_1b220();
      iVar1 = extraout_A0_00;
    }
    *(ushort *)(iVar1 + 0x92) = *(byte *)(unaff_A6 + 0x73) & 0xf;
    *(undefined2 *)(iVar1 + 0x8e) = *(undefined2 *)(unaff_A6 + 0x8e);
  } while ('\0' < *(char *)(unaff_A6 + 0x73));
  return;
}
