/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0725f0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_725f0(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_4256();
  candidate_9ea8();
  *unaff_A6 = 0x72614;
  bVar3 = false;
  candidate_409a();
  if (bVar3) {
    candidate_72770();
    candidate_1ad6e();
    candidate_1aca2();
    *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(extraout_A0 + 0x1e);
    *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -1;
    candidate_1abc8();
    candidate_427a();
    return;
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
