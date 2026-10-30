/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08df42.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8df42(void)

{
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_1b1dc();
  candidate_1af9a();
  *(undefined4 *)((int)unaff_A6 + 0x7a) = extraout_A0;
  *(undefined1 *)((int)unaff_A6 + 0x8d) = 1;
  *(undefined1 *)((int)unaff_A6 + 0x8f) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
  candidate_4256();
  *unaff_A6 = 0x8df76;
  candidate_8f4ce();
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x8e) != '\0') {
    *unaff_A6 = candidate_8df92;
  }
  uVar1 = 0;
  candidate_8e15a();
  *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) & 0xbf;
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 0x40;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_8e0d4;
    uVar1 = 0;
  }
  candidate_8f432();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_8e128;
  }
  return;
}
