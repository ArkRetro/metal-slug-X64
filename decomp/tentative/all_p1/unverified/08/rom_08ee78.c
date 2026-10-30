/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08ee78.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8ee78(void)

{
  code *UNRECOVERED_JUMPTABLE;
  undefined4 *unaff_A6;
  bool bVar1;

  bVar1 = *(byte *)((int)unaff_A6 + 0x8d) < 7;
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
    bVar1 = false;
    candidate_8f432();
    if (bVar1) {
      *unaff_A6 = candidate_41ac;
    }
    return;
  }
  candidate_8f88a();
  if (bVar1) {
                    /* WARNING: Could not recover jumptable at 0x0008ee8a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)();
    return;
  }
  candidate_8f3e2();
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -1;
  candidate_427a();
  return;
}
