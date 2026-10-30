/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ccba8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ccba8(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined2 *)(unaff_A6 + 0x2a) = 0x100;
  *(undefined1 *)(unaff_A6 + 0x20) = 2;
  *(undefined2 *)((int)unaff_A6 + 0x82) = 600;
  candidate_4256();
  *unaff_A6 = 0xccbe8;
  uVar1 = false;
  candidate_ce448();
  if ((bool)uVar1) {
    *(undefined1 *)(unaff_A6 + 0x20) = 3;
    uVar1 = 0;
  }
  candidate_ce420();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 1;
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
