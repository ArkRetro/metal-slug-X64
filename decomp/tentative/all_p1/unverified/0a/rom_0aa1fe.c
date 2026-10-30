/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa1fe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aa1fe(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_120f8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8848;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8848;
  candidate_19dfc();
  *(undefined2 *)(unaff_A6 + 0x1c) = 9;
  if (iRam000e685a != 0) {
    candidate_4256();
    *(short *)(unaff_A6 + 0x1c) = *(short *)(unaff_A6 + 0x1c) + 1;
  }
  candidate_4e76();
  *unaff_A6 = 0xaa252;
  uVar3 = 0;
  candidate_1436a();
  candidate_1ad8c();
  candidate_427a();
  candidate_140c0();
  candidate_1945c();
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
