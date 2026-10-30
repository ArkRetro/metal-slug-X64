/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x032040.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_32040(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  ushort uVar2;

  iVar1 = *(int *)((int)unaff_A6 + 0x62);
  if (0x8000 < (ushort)iVar1) {
    iVar1 = iVar1 + 0x10000;
  }
  uVar2 = (ushort)((uint)iVar1 >> 0x10);
  if (10 < uVar2) {
    uVar2 = 10;
  }
  uVar3 = *(short *)((short)(uVar2 * 2) + 0x31b86) < 0;
  candidate_4256();
  candidate_29c6();
  if ((bool)uVar3) {
    *(int *)((int)unaff_A6 + 0x62) = -*(int *)((int)unaff_A6 + 0x62);
  }
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  if (-1 < *(short *)((int)unaff_A6 + 0x62)) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_120f8();
  *unaff_A6 = 0x320aa;
  candidate_320d0();
  candidate_8ccd4();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *(int *)((int)unaff_A6 + 0x62) = -*(int *)((int)unaff_A6 + 0x62);
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  }
  candidate_427a();
  return;
}
