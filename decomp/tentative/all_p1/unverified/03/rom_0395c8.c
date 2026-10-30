/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0395c8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_395c8(void)

{
  undefined4 uVar1;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  uVar1 = candidate_39d74();
  if ((CONCAT31((int3)((uint)uVar1 >> 8),*(byte *)(unaff_A6 + 0x25) ^ *(byte *)(unaff_A5 + 0x4606))
      & 1 << (*(ushort *)(unaff_A6 + 0x1d) & 0x1f)) != 0) {
    *unaff_A6 = candidate_3965c;
  }
  uVar2 = 0;
  candidate_39b4a();
  candidate_4c24();
  if ((bool)uVar2) {
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
