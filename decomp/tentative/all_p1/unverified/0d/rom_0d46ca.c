/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d46ca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d46ca(void)

{
  short sVar1;
  char cVar2;
  short sVar3;
  undefined2 uVar4;
  undefined4 *unaff_A6;

  uVar4 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x1d) = uVar4;
  *unaff_A6 = 0xd46e0;
  sVar1 = *(short *)(unaff_A6 + 0x1d);
  sVar3 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x1d) = sVar3;
  if (sVar3 == 0 || sVar1 < 1) {
    *unaff_A6 = 0xd44ca;
  }
  cVar2 = *(char *)(unaff_A6[3] + (int)*(short *)((int)unaff_A6 + 0x8e));
  *(undefined1 *)(unaff_A6[3] + (int)*(short *)((int)unaff_A6 + 0x8e)) = 0;
  if (cVar2 == '\x05') {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
