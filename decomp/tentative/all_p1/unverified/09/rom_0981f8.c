/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0981f8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_981f8(void)

{
  code *pcVar1;
  undefined2 extraout_var;
  short sVar2;
  undefined2 extraout_var_00;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int iVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  if (*(char *)((int)unaff_A6 + 0x8d) == '\0') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  sVar2 = candidate_29c6();
  if (-1 < sVar2) {
    candidate_1da40();
    *(undefined2 *)(extraout_A0 + 0x66) = 0xe0ed;
    *(undefined2 *)(extraout_A0 + 0x1e) = 0xe0ed;
    candidate_2eb4();
    candidate_29c6();
    candidate_2e7c();
    *(undefined2 *)((int)unaff_A6 + 0x72) = extraout_var;
    *(undefined2 *)(unaff_A6 + 0x1d) = extraout_var_00;
    *unaff_A6 = 0x98288;
    candidate_98770();
    candidate_9861e();
    if (*(short *)(unaff_A6 + 0x10) < 0x120) {
      *unaff_A6 = candidate_982a4;
    }
    bVar4 = false;
    candidate_1945c();
    if (bVar4) {
      *unaff_A6 = 0x9841e;
    }
    return;
  }
  candidate_120f8();
  *(byte *)(unaff_A6 + 0x23) = *(byte *)(unaff_A6 + 0x23) | 4;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  bVar4 = false;
  candidate_40bc();
  if (bVar4) {
    candidate_1b220();
  }
  candidate_1da40();
  bVar4 = false;
  candidate_40bc();
  if (bVar4) {
    candidate_1b220();
  }
  bVar4 = false;
  candidate_40bc();
  if (bVar4) {
    candidate_1b220();
  }
  bVar4 = false;
  candidate_40bc();
  if (bVar4) {
    candidate_1b220();
  }
  bVar4 = false;
  candidate_40bc();
  iVar3 = extraout_A0_00;
  if (bVar4) {
    candidate_1b220();
    iVar3 = extraout_A0_01;
  }
  *(byte *)(iVar3 + 0x15) = *(byte *)(iVar3 + 0x15) ^ 1;
  *(byte *)(unaff_A6 + 0x23) = *(byte *)(unaff_A6 + 0x23) | 4;
  candidate_4126();
  iVar3 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar3 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
