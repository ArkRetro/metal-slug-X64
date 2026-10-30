/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03963e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3963e(void)

{
  undefined4 uVar1;
  undefined2 uVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_39e6c();
  candidate_1b1dc();
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x86) = uVar2;
  uVar1 = candidate_39d74();
  if ((CONCAT31((int3)((uint)uVar1 >> 8),*(byte *)(unaff_A6 + 0x25) ^ *(byte *)(unaff_A5 + 0x4606))
      & 1 << (*(ushort *)(unaff_A6 + 0x1d) & 0x1f)) != 0) {
    *unaff_A6 = candidate_3965c;
  }
  uVar3 = 0;
  candidate_39b4a();
  candidate_4c24();
  if ((bool)uVar3) {
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
