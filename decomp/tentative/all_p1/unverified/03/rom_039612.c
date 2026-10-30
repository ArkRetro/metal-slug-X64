/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x039612.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_39612(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  sVar1 = *(short *)(unaff_A6 + 10);
  candidate_39d74();
  sVar1 = sVar1 - *(short *)(unaff_A6 + 10);
  if (sVar1 < 0) {
    sVar1 = -sVar1;
  }
  if ((short)(*(short *)(unaff_A6 + 0x1d) - sVar1) < 0) {
    *unaff_A6 = candidate_3963e;
  }
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) - sVar1;
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
