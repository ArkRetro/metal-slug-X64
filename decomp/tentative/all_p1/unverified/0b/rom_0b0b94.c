/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b0b94.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b0b94(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(undefined4 *)((int)unaff_A6 + 0x6e) = *(undefined4 *)(*(int *)((int)unaff_A6 + 0x72) + 0x2e);
  *unaff_A6 = 0xb0ba4;
  iVar2 = *(int *)((int)unaff_A6 + 0x72);
  if (*(int *)(iVar2 + 0x2e) != *(int *)((int)unaff_A6 + 0x6e)) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  while (*(short *)((int)unaff_A6 + 0x76) <=
         (short)((*(short *)(iVar2 + 0x36) + 0x20) - *(short *)(iVar2 + 0x32))) {
    func_0x000c6f82();
    iVar2 = extraout_A0;
  }
  return;
}
