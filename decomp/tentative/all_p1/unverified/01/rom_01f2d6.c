/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01f2d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1f2d6(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_146e8();
  candidate_9f88();
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8844;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8844;
  candidate_4256();
  *unaff_A6 = 0x1f322;
  uVar1 = 0;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar1) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar1 = 0;
    }
    else {
      candidate_406b6();
    }
  }
  else {
    uVar1 = *(char *)(unaff_A6[3] + 0x61) != -1;
    if (*(char *)(unaff_A6[3] + 0x61) == -1) {
      *unaff_A6 = candidate_b05fe;
      uVar1 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_1f75a;
  }
  return;
}
