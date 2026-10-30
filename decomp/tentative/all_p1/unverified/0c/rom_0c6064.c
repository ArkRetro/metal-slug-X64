/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c6064.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c6064(void)

{
  int iVar1;
  undefined4 *unaff_A6;

  *(undefined1 *)((int)unaff_A6 + 0x96) = 0;
  candidate_29c6();
  candidate_1ad6e();
  candidate_9f88();
  candidate_9ea8();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)(unaff_A6 + 0x23) = 0;
  candidate_1b1dc();
  if ((*(byte *)(unaff_A6[3] + 0x15) & 1) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_4256();
  *unaff_A6 = 0xc610e;
  candidate_1abc8();
  candidate_c6bc4();
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)(unaff_A6[3] + 0x1e) + -1;
  candidate_427a();
  iVar1 = *(int *)(unaff_A6[3] + 0xc);
  if (*(char *)(iVar1 + 0x97) == '\x01') {
    *unaff_A6 = candidate_c616c;
  }
  if (((*(char *)(iVar1 + 0x97) == '\x06') || (*(char *)(iVar1 + 0x97) == '\x05')) ||
     (*(char *)(iVar1 + 0x97) == '\a')) {
    *unaff_A6 = candidate_c66e8;
  }
  return;
}
