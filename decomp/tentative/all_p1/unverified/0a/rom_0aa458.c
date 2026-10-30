/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa458.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aa458(void)

{
  code *pcVar1;
  short *psVar2;
  int unaff_A5;
  int unaff_A6;
  bool bVar3;

  if (*(short *)(unaff_A6 + 0x70) == 0) {
    psVar2 = (short *)(unaff_A5 + 0x4608);
    pcVar1 = candidate_42e8;
  }
  else {
    psVar2 = (short *)(unaff_A5 + 0x462c);
    pcVar1 = thunk_FUN_000044bc;
  }
  if (*psVar2 == 0) {
    *(byte *)(psVar2 + 0xe) = *(byte *)(psVar2 + 0xe) >> 1;
  }
  else {
    if ((*(char *)(psVar2 + 0xe) != '\0') &&
       (bVar3 = (ushort)psVar2[9] < *(ushort *)(unaff_A5 + -0x7ff2),
       psVar2[9] == *(ushort *)(unaff_A5 + -0x7ff2))) {
      candidate_18f24();
      if (!bVar3) {
        *(undefined1 *)(psVar2 + 0xe) = 0;
        return;
      }
    }
    *psVar2 = 0;
    *(undefined1 *)(psVar2 + 0xe) = 1;
  }
  *(undefined4 *)(pcVar1 + unaff_A5) = 0x16dbe;
  return;
}
