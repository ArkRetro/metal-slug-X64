/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08ebb6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8ebb6(void)

{
  byte bVar1;
  code *UNRECOVERED_JUMPTABLE;
  undefined4 *unaff_A6;
  bool bVar2;

  bVar2 = *(byte *)((int)unaff_A6 + 0x8d) < 7;
  if (*(byte *)((int)unaff_A6 + 0x8d) == 7) {
    *(undefined1 *)((int)unaff_A6 + 0x17) = 0;
    unaff_A6[0xd] = 0x66666;
    *(undefined4 *)((int)unaff_A6 + 0x76) = 0xffff92c6;
    unaff_A6[0xc] = 0xffffbbbc;
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
    *unaff_A6 = 0x8eeea;
    unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xd];
    func_0x000143e4();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_8f1e0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
      *unaff_A6 = candidate_8f1e0;
    }
    candidate_427a();
    if (*(short *)(unaff_A6 + 0x10) < 0xc1) {
      *unaff_A6 = candidate_8ef46;
    }
    bVar2 = false;
    candidate_8f432();
    if (bVar2) {
      *unaff_A6 = candidate_41ac;
    }
    return;
  }
  candidate_8f88a();
  if (!bVar2) {
    candidate_8f3e2();
    if (*(char *)((int)unaff_A6 + 0x8e) == '\0') {
      *(char *)((int)unaff_A6 + 0x8f) = *(char *)((int)unaff_A6 + 0x8f) + '\x01';
      bVar1 = *(byte *)((int)unaff_A6 + 0x8f) & 3;
      *(byte *)((int)unaff_A6 + 0x8f) = bVar1;
    }
    else {
      *(char *)((int)unaff_A6 + 0x8f) = *(char *)((int)unaff_A6 + 0x8f) + '\x01';
      bVar1 = *(byte *)((int)unaff_A6 + 0x8f) & 0x3f;
      *(byte *)((int)unaff_A6 + 0x8f) = bVar1;
    }
    if (bVar1 == 0) {
      candidate_8f456();
      *(undefined1 *)(unaff_A6[3] + 0x8e) = 0;
      bVar2 = false;
      candidate_8f98e();
      if (bVar2) {
        *(undefined1 *)(unaff_A6[3] + 0x8e) = 1;
      }
      *(bool *)((int)unaff_A6 + 0x8e) = bVar2;
    }
    bVar2 = false;
    candidate_427a();
    if (bVar2) {
      candidate_4256();
    }
    if ((*(byte *)(unaff_A6[3] + 0x93) & 0x40) != 0) {
      candidate_4256();
    }
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x0008ebc8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}
