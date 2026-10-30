/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa760.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_aa760(void)

{
  undefined2 extraout_D0u;
  char in_D0b;
  short unaff_D2w;
  short unaff_D3w;
  int extraout_A0;
  short *psVar1;
  int unaff_A5;
  int unaff_A6;
  bool bVar2;

  bVar2 = false;
  if (in_D0b == '\0') {
    psVar1 = (short *)(unaff_A5 + 0x4608);
  }
  else {
    psVar1 = (short *)(unaff_A5 + 0x462c);
  }
  func_0x000040ac();
  if ((bVar2) && ((*(byte *)(extraout_A0 + 0x5f) & 0x80) == 0)) {
    if (((*psVar1 == 0) || ((short)unaff_A6 == *psVar1)) && (*(char *)(psVar1 + 0xe) == '\0')) {
      *psVar1 = (short)unaff_A6;
      *(undefined1 *)(psVar1 + 0xe) = 0xff;
      psVar1[9] = *(short *)(unaff_A5 + -0x7ff2);
      psVar1[1] = unaff_D2w;
      psVar1[2] = unaff_D3w;
      *(undefined4 *)(psVar1 + 3) = *(undefined4 *)(unaff_A6 + 0x38);
      *(undefined4 *)(psVar1 + 5) = *(undefined4 *)(unaff_A6 + 0x28);
      *(undefined4 *)(psVar1 + 7) = *(undefined4 *)(unaff_A6 + 0x2c);
      if (*(char *)(unaff_A6 + 0x5a) != '\0') {
        *(char *)(psVar1 + 0xf) = *(char *)(unaff_A6 + 0x5a);
      }
      *(undefined1 *)(unaff_A6 + 0x5e) = *(undefined1 *)(extraout_A0 + 0x5e);
      *(undefined1 *)((int)psVar1 + 0x1d) = 1;
      *(undefined1 *)(psVar1 + 0x11) = *(undefined1 *)(unaff_A6 + 0x15);
      return CONCAT22(extraout_D0u,*(undefined2 *)(extraout_A0 + 0x6e));
    }
  }
  return 0;
}
