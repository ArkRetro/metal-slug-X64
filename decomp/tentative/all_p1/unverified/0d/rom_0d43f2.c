/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d43f2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d43f2(void)

{
  code *UNRECOVERED_JUMPTABLE;
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined2 *)(unaff_A6 + 0x23) = 0xffff;
  *(undefined2 *)((int)unaff_A6 + 0x8e) = 0x81;
  unaff_A6[0x22] = 0xe8e60;
  *(undefined2 *)(unaff_A6 + 0x24) = 2;
  candidate_4256();
  candidate_1ad6e();
  *(undefined1 *)((int)unaff_A6 + 0x7b) = *(undefined1 *)(unaff_A6[3] + 0x7b);
  candidate_d5498();
  candidate_4256();
  *unaff_A6 = candidate_d42e2;
  *(undefined1 *)(unaff_A6 + 0x18) = *(undefined1 *)(unaff_A6[3] + 0x60);
  uVar1 = 0;
  candidate_d5506();
  candidate_d54ee();
  candidate_d5530();
  if ((bool)uVar1) {
                    /* WARNING: Could not recover jumptable at 0x000d4302. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)();
    return;
  }
  candidate_44f6();
  candidate_427a();
  return;
}
