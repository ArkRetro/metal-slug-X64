/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0be498.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_be498(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_1ac38();
  if ((*(char *)((int)unaff_A6 + 0x72) != '\0') &&
     (*(short *)(unaff_A6 + 0xb) <= *(short *)((int)unaff_A6 + 0x6e))) {
    *(undefined1 *)((int)unaff_A6 + 0x72) = 0;
  }
  if ((*(char *)((int)unaff_A6 + 0x73) == '\0') &&
     (*(short *)(unaff_A6 + 0xb) <= *(short *)(unaff_A6 + 0x1c))) {
    *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0x1c);
    unaff_A6[0xd] = 0;
    *(undefined1 *)((int)unaff_A6 + 0x72) = 0xff;
    candidate_4256();
  }
  uVar1 = 0;
  candidate_140c0();
  candidate_427a();
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
