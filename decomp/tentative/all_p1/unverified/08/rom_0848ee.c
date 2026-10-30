/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0848ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_848ee(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x84918;
  sVar3 = *(short *)(unaff_A6[3] + 0x1e);
  *(short *)((int)unaff_A6 + 0x66) = *(short *)(unaff_A6[3] + 0x66) + -1;
  *(short *)((int)unaff_A6 + 0x1e) = sVar3 + -1;
  uVar4 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar4) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
