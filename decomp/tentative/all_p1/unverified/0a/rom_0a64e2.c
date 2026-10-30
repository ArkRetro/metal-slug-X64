/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a64e2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a64e2(void)

{
  code *pcVar1;
  int iVar2;
  undefined4 extraout_D1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  candidate_4256();
  candidate_1ad6e();
  candidate_a6574();
  *unaff_A6 = 0xa6504;
  bVar3 = false;
  candidate_a658a();
  if (bVar3) {
    if ((*(byte *)((int)unaff_A6 + 0x86) & 8) != 0) {
      candidate_1ac4c();
      unaff_A6[0xb] = extraout_D1;
    }
    uVar4 = 0;
    if ((*(byte *)((int)unaff_A6 + 0x86) & 0x10) != 0) {
      uVar4 = 0;
      candidate_1ad6e();
      candidate_a6574();
      candidate_146e8();
    }
    candidate_1abc8();
    candidate_427a();
    if ((bool)uVar4) {
      *unaff_A6 = 0xa656c;
    }
    return;
  }
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
