/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x039796.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_39796(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0x397b0;
  uVar1 = false;
  candidate_39d74();
  if ((bool)uVar1) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
    *(undefined1 *)(unaff_A6 + 0x24) = 0;
    *unaff_A6 = 0x39682;
    uVar1 = 0;
  }
  candidate_39b4a();
  candidate_4c24();
  if ((bool)uVar1) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    else {
      *unaff_A6 = candidate_39820;
    }
  }
  if (*(char *)((int)unaff_A6 + 0x93) != '\0') {
    *(char *)((int)unaff_A6 + 0x93) = *(char *)((int)unaff_A6 + 0x93) + -1;
  }
  return;
}
