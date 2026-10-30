/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x065de6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_65de6(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  unaff_A6[10] = 0x1400000;
  unaff_A6[0xb] = 0x1800000;
  unaff_A6[0xc] = 0xfff80000;
  unaff_A6[0xd] = 0;
  candidate_4256();
  *unaff_A6 = 0x65e32;
  candidate_1ac38();
  sVar3 = candidate_49e8();
  if (sVar3 == 0) {
    candidate_427a();
    return;
  }
  bVar4 = false;
  candidate_40bc();
  if (bVar4) {
    candidate_1b220();
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
