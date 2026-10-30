/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03f23a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3f23a(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  candidate_4256();
  *unaff_A6 = 0x3f250;
  uVar2 = 0;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  if ((bool)uVar2) {
    bVar3 = false;
    candidate_3fc26();
    if (bVar3) {
      uVar1 = candidate_29c6();
      *(undefined2 *)(unaff_A6 + 0x1c) = 0x200;
      *(ushort *)(unaff_A6 + 0x1c) = (uVar1 & 0xff) + *(short *)(unaff_A6 + 0x1c);
      *(undefined2 *)((int)unaff_A6 + 0x6e) = 1;
      candidate_3faf2();
      candidate_3fb10();
      candidate_3f3f2();
      *unaff_A6 = candidate_3f1fe;
    }
    else {
      candidate_3f31e();
    }
  }
  candidate_3fada();
  return;
}
