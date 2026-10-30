/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04ba16.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4ba16(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  candidate_120f8();
  *(undefined1 *)((int)unaff_A6 + 0x9e) = 0;
  if (*(char *)(*(int *)(unaff_A6[3] + 0xc) + 0xa4) == '\x01') {
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x400;
  }
  else {
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x200;
  }
  *unaff_A6 = 0x4ba5c;
  uVar1 = 0;
  candidate_1abc8();
  candidate_4d9de();
  if ((bool)uVar1) {
    if (*(char *)((int)unaff_A6 + 0x9d) == '\x06') {
      *unaff_A6 = candidate_4ba94;
      uVar1 = 0;
    }
    else {
      *unaff_A6 = candidate_4beec;
      uVar1 = 0;
    }
  }
  candidate_4d74a();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  }
  return;
}
