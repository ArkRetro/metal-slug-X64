/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01d908.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1d908(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_1dd90();
  *(char *)(unaff_A6 + 0x23) = *(char *)(unaff_A6 + 0x23) + '\x01';
  uVar1 = 0;
  if ((*(byte *)(unaff_A6 + 0x23) & 0xf) == 0) {
    candidate_1dde8();
  }
  else {
    uVar1 = 0;
    if ((*(byte *)(unaff_A6 + 0x23) & 3) == 0) {
      candidate_1de0e();
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_4188;
    uVar1 = 0;
  }
  candidate_409a();
  if (!(bool)uVar1) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
