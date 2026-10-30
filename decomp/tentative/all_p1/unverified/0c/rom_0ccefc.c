/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ccefc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ccefc(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *unaff_A6 = 0xccf12;
  uVar1 = false;
  candidate_ce448();
  if ((bool)uVar1) {
    *(undefined1 *)(unaff_A6 + 0x20) = 4;
    uVar1 = 0;
  }
  candidate_ce420();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_cca24;
  }
  candidate_ce5da();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *(undefined1 *)((int)unaff_A6 + 0x73) = 2;
  }
  return;
}
