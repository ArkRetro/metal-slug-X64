/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d2ba8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d2ba8(void)

{
  uint uVar1;
  char cVar2;
  uint uVar3;
  undefined2 uVar4;
  short sVar5;
  undefined4 *unaff_A6;
  undefined1 uVar6;
  bool bVar7;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  uVar4 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar4;
  unaff_A6[0x15] = 0xe830c;
  candidate_4256();
  *unaff_A6 = 0xd2bf0;
  uVar3 = (int)((uint)*(ushort *)(unaff_A6 + 0x22) << 0x10) >> 8;
  uVar1 = unaff_A6[0xd];
  uVar6 = CARRY4(uVar3,uVar1);
  unaff_A6[0xd] = uVar3 + uVar1;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_d2d14;
    uVar6 = 0;
  }
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar6) {
    cVar2 = *(char *)((int)unaff_A6 + 0x5a);
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    if ((cVar2 == '\x03') || (cVar2 == '\x06')) {
      if (cVar2 == '\x03') {
        unaff_A6[0xd] = unaff_A6[0xd] + -0x80000;
      }
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      uVar4 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar4;
      *unaff_A6 = candidate_d2cb6;
    }
  }
  sVar5 = (short)*(int *)((int)unaff_A6 + 0x82);
  if (*(int *)((int)unaff_A6 + 0x82) != 0) {
    sVar5 = candidate_49e8();
  }
  if (sVar5 != 0) {
    *unaff_A6 = candidate_d289a;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d289a;
  }
  bVar7 = false;
  candidate_19504();
  if (bVar7) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
