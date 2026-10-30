/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c5c60.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c5c60(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_1b1dc();
  unaff_A6[0x1e] = 0x2000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    unaff_A6[0x1e] = -unaff_A6[0x1e];
  }
  *(undefined2 *)(unaff_A6 + 0x23) = 0;
  unaff_A6[0x22] = unaff_A6[0x21];
  unaff_A6[0x21] = 0;
  candidate_4256();
  *unaff_A6 = 0xc5ca6;
  uVar1 = false;
  candidate_c6a46();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x97) = 4;
    uVar1 = 0;
  }
  candidate_c6a02();
  candidate_c6a1e();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x97) = 4;
  }
  candidate_427a();
  candidate_c699c();
  if (*(byte *)((int)unaff_A6 + 0x97) == 4) {
    *unaff_A6 = candidate_c5b8c;
  }
  uVar1 = *(byte *)((int)unaff_A6 + 0x97) < 4;
  candidate_4c24();
  if ((bool)uVar1) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_c5d26;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_c5dcc;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_c66e8;
  }
  return;
}
