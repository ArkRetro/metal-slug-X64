/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0989de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_989de(void)

{
  ushort uVar1;
  byte bVar3;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar4;
  bool bVar5;
  undefined1 uVar6;
  undefined8 uVar7;

  uVar1 = *(ushort *)((int)unaff_A6 + 0x86) - 1;
  *(ushort *)((int)unaff_A6 + 0x86) = uVar1;
  if (((int)((uint)uVar1 << 0x10) < 0) || (*(int *)((int)unaff_A6 + 0x6e) == 0)) {
    bVar3 = candidate_2eb4();
    bVar4 = (char)bVar3 < '\0';
    bVar3 = (-bVar4 ^ bVar3) + bVar4 + 0x40;
    bVar5 = (int)((uint)bVar3 << 0x18) < 0;
    bVar3 = ((-bVar5 ^ bVar3) + bVar5) - 0x40;
    if (0x20 < bVar3) {
      bVar3 = 0x20;
    }
    *(byte *)((int)unaff_A6 + 0x8f) = (-bVar4 ^ ((-bVar5 ^ bVar3 + 0x40) + bVar5) - 0x40) + bVar4;
    *unaff_A6 = 0x98a62;
    uVar7 = candidate_2e7c();
    unaff_A6[0xc] = ((int)((ulonglong)uVar7 >> 0x20) - unaff_A6[0xc] >> 3) + unaff_A6[0xc];
    unaff_A6[0xd] = ((int)uVar7 - unaff_A6[0xd] >> 3) + unaff_A6[0xd];
    candidate_1ac38();
    candidate_427a();
  }
  else {
    uVar1 = *(ushort *)(unaff_A6 + 0x20) - 1;
    *(ushort *)(unaff_A6 + 0x20) = uVar1;
    if ((int)((uint)uVar1 << 0x10) < 0) {
      *(undefined2 *)(unaff_A6 + 0x20) = *(undefined2 *)(unaff_A6 + 0x22);
      candidate_98b32();
      candidate_98baa();
    }
    candidate_98bca();
    candidate_427a();
  }
  bVar3 = *(byte *)(unaff_A6 + 0x24);
  uVar6 = bVar3 < 0x40;
  *(byte *)(unaff_A6 + 0x24) = bVar3 - 0x40;
  if ((bool)uVar6) {
    candidate_98fbe();
  }
  candidate_4c24();
  if ((bool)uVar6) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *unaff_A6 = candidate_98afe;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_120f8();
    *unaff_A6 = candidate_98afe;
  }
  bVar4 = false;
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
