/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08ec54.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8ec54(void)

{
  int iVar1;
  byte bVar2;
  char cVar3;
  code *UNRECOVERED_JUMPTABLE;
  undefined4 *unaff_A6;
  bool bVar4;

  bVar4 = *(byte *)((int)unaff_A6 + 0x8d) < 7;
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
    bVar4 = false;
    candidate_8f432();
    if (bVar4) {
      *unaff_A6 = candidate_41ac;
    }
    return;
  }
  candidate_8f88a();
  if (bVar4) {
                    /* WARNING: Could not recover jumptable at 0x0008ec66. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)();
    return;
  }
  candidate_8f3e2();
  *(char *)((int)unaff_A6 + 0x8f) = *(char *)((int)unaff_A6 + 0x8f) + '\x01';
  bVar2 = *(byte *)((int)unaff_A6 + 0x8f) & 3;
  *(byte *)((int)unaff_A6 + 0x8f) = bVar2;
  if (bVar2 == 0) {
    iVar1 = unaff_A6[3];
    if (*(char *)(iVar1 + 0x8a) == *(char *)((int)unaff_A6 + 0x8a)) {
      *(undefined1 *)(iVar1 + 0x8e) = 1;
    }
    else {
      if (*(char *)(iVar1 + 0x8a) < *(char *)((int)unaff_A6 + 0x8a)) {
        cVar3 = -1;
      }
      else {
        cVar3 = '\x01';
      }
      *(char *)((int)unaff_A6 + 0x8a) = cVar3 + *(char *)((int)unaff_A6 + 0x8a);
      *(undefined1 *)(iVar1 + 0x8e) = 0;
    }
  }
  bVar4 = false;
  candidate_427a();
  if (bVar4) {
    candidate_4256();
  }
  if ((*(byte *)(unaff_A6[3] + 0x93) & 0x40) != 0) {
    candidate_4256();
  }
  return;
}
