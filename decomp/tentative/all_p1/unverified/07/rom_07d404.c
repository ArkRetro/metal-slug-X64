/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07d404.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7d404(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_13f1e();
  candidate_120f8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_40bc();
  candidate_1acf2();
  candidate_1b1dc();
  candidate_4256();
  *unaff_A6 = 0x7d45a;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_140c0();
  candidate_7d8f0();
  if ((bool)uVar3) {
    candidate_13f1e();
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
