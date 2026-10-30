/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ad6e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ad6e4(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_1ad6e();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0xad712;
  candidate_1abc8();
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  sVar3 = *(short *)((int)unaff_A6 + 0x1e);
  bVar4 = sVar3 == 0;
  *(short *)((int)unaff_A6 + 0x1e) = sVar3 + -1;
  candidate_427a();
  if (bVar4) {
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
