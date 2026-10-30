/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f58c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f58c(void)

{
  uint uVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 1;
  candidate_9f88();
  candidate_1b2be();
  if ((bool)in_CF) {
    candidate_9ea8();
  }
  else {
    candidate_9ea8();
  }
  if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  uVar1 = candidate_29c6();
  if ((uVar1 & 1) == 0) {
    *unaff_A6 = candidate_9e920;
  }
  else {
    *unaff_A6 = candidate_9eab8;
  }
  return;
}
