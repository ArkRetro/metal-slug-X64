/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04be74.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4be74(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  candidate_120f8();
  if (*(char *)(*(int *)(unaff_A6[3] + 0xc) + 0xa4) == '\x01') {
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x400;
  }
  else {
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x200;
  }
  *unaff_A6 = 0x4bebc;
  uVar1 = 0;
  candidate_1abc8();
  candidate_4d9de();
  if ((bool)uVar1) {
    *(undefined1 *)(*(int *)(unaff_A6[3] + 0xc) + 0x9e) = 0;
    *unaff_A6 = candidate_4b97e;
    uVar1 = 0;
  }
  candidate_4d74a();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  }
  return;
}
