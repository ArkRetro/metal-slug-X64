/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03f392.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3f392(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar2;

  candidate_3faba();
  candidate_427a();
  if ((bool)in_CF) {
    bVar2 = false;
    candidate_3fc26();
    if (bVar2) {
      uVar1 = candidate_29c6();
      *(undefined2 *)(unaff_A6 + 0x1c) = 0x200;
      *(ushort *)(unaff_A6 + 0x1c) = (uVar1 & 0xf0) + *(short *)(unaff_A6 + 0x1c);
      *(undefined2 *)((int)unaff_A6 + 0x6e) = 1;
      candidate_3faf2();
      candidate_3fb10();
      candidate_3f3f2();
      *unaff_A6 = candidate_3f1fe;
    }
    else {
      candidate_4256();
    }
  }
  candidate_3fada();
  return;
}
