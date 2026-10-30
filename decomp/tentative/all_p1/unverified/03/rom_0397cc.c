/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0397cc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_397cc(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4256();
  uVar2 = candidate_39eda();
  *(undefined2 *)((int)unaff_A6 + 0x82) = uVar2;
  *(undefined2 *)(unaff_A6 + 0x21) = uVar2;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  unaff_A6[0xc] = 0;
  *unaff_A6 = 0x397f6;
  candidate_39d74();
  sVar1 = *(short *)(unaff_A6 + 0x21);
  *(short *)(unaff_A6 + 0x21) = sVar1 + -1;
  if (sVar1 == 0) {
    *(ushort *)((int)unaff_A6 + 0x72) = *(ushort *)((int)unaff_A6 + 0x72) | 2;
    *unaff_A6 = candidate_3965c;
  }
  uVar3 = false;
  candidate_39e9a();
  if (!(bool)uVar3) {
    *unaff_A6 = candidate_3965c;
    uVar3 = 0;
  }
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
