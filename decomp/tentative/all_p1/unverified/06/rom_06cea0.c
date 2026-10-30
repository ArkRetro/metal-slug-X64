/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06cea0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6cea0(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  undefined4 extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar4;

  candidate_1436a();
  candidate_427a();
  if ((bool)in_CF) {
    candidate_4256();
  }
  sVar3 = candidate_8c0c2();
  if (-1 < sVar3) {
    candidate_8ce42();
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
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
  bVar4 = false;
  candidate_1affc();
  if (!bVar4) {
    *(undefined4 *)((int)unaff_A6 + 0x6e) = extraout_A0;
  }
  bVar4 = false;
  if (*(short *)(unaff_A6 + 0x1f) != 0) {
    sVar3 = *(short *)(unaff_A6 + 0x1f);
    bVar4 = sVar3 == 0;
    *(short *)(unaff_A6 + 0x1f) = sVar3 + -1;
  }
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = 0x6d022;
  }
  return;
}
