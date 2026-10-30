/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d1ed8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d1ed8(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined1 *)((int)unaff_A6 + 0x77) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0;
  unaff_A6[0xc] = 0;
  candidate_4256();
  *unaff_A6 = 0xd1f06;
  uVar1 = 0;
  candidate_1abc8();
  candidate_d3256();
  candidate_d31be();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_d1cd2;
    uVar1 = 0;
  }
  candidate_427a();
  candidate_d338a();
  candidate_d3280();
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d1f2e;
  }
  return;
}
