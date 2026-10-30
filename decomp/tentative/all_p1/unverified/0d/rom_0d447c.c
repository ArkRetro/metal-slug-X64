/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d447c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d447c(void)

{
  byte bVar1;
  ushort uVar2;
  code *UNRECOVERED_JUMPTABLE;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_1ad6e();
  candidate_4c1c();
  *(undefined2 *)(unaff_A6 + 0x23) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x8e) = 0x7e;
  unaff_A6[0x22] = 0xe8ec0;
  *(undefined2 *)(unaff_A6 + 0x24) = 5;
  uVar2 = candidate_29c6();
  *(ushort *)((int)unaff_A6 + 0x76) = (uVar2 & 0x3f) + 0x1e;
  candidate_9ea8();
  *(undefined1 *)((int)unaff_A6 + 0x92) = 1;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  candidate_4256();
  candidate_427a();
  *unaff_A6 = 0xd44fc;
  *(undefined1 *)(unaff_A6 + 0x18) = *(undefined1 *)(unaff_A6[3] + 0x60);
  bVar3 = false;
  candidate_d545c();
  if (bVar3) {
    *unaff_A6 = candidate_d45d4;
  }
  bVar1 = *(byte *)(unaff_A6[3] + (int)*(short *)((int)unaff_A6 + 0x8e));
  if (bVar1 == 5) {
    *unaff_A6 = candidate_d470e;
  }
  bVar3 = bVar1 < 5;
  candidate_d5530();
  if (bVar3) {
                    /* WARNING: Could not recover jumptable at 0x000d453a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)();
    return;
  }
  return;
}
