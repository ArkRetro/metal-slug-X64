/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08b138.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8b138(void)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  if (*(char *)(unaff_A6 + 0x20) == '\0') {
    *(byte *)((int)unaff_A6 + 0x7f) = *(byte *)((int)unaff_A6 + 0x7f) | 2;
                    /* WARNING: Could not recover jumptable at 0x0008b19c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)((short)(*(short *)((int)unaff_A6 + 0x6e) * 4) + 0x8b1a0))();
    return;
  }
  *(byte *)((int)unaff_A6 + 0x7f) = *(byte *)((int)unaff_A6 + 0x7f) | 1;
  uVar3 = candidate_4256();
  unaff_A6[0xd] = uVar3;
  *unaff_A6 = 0x8b170;
  uVar2 = *(short *)((int)unaff_A6 + 0x7a) >> 4;
  uVar1 = *(ushort *)((int)unaff_A6 + 0x7a);
  uVar4 = uVar1 < uVar2;
  *(ushort *)((int)unaff_A6 + 0x7a) = uVar1 - uVar2;
  candidate_8b2de();
  candidate_8b31a();
  candidate_427a();
  candidate_1945c();
  if ((bool)uVar4) {
    *unaff_A6 = 0x8b250;
  }
  return;
}
