/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03a09c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3a09c(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  if (*(ushort *)(unaff_A6 + 0x1c) <= *(ushort *)(unaff_A5 + 0x699e)) {
    candidate_4192();
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  sVar3 = *(short *)((int)unaff_A6 + 0x76) + -1;
  *(short *)((int)unaff_A6 + 0x76) = sVar3;
  if (-1 < sVar3) {
    return;
  }
  *(undefined2 *)((int)unaff_A6 + 0x76) = *(undefined2 *)(unaff_A6 + 0x1d);
  uVar4 = 0;
  candidate_1abc8();
  candidate_19504();
  if ((bool)uVar4) {
    return;
  }
  candidate_3a0e4();
  candidate_120f8();
  return;
}
