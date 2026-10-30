/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06f820.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6f820(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  byte bVar4;
  uint uVar5;
  short sVar6;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar7;
  undefined8 uVar8;

  candidate_4256();
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -1;
  uVar8 = candidate_2e7c();
  iVar2 = *(int *)(unaff_A6[3] + 0x34);
  unaff_A6[0xc] = *(int *)(unaff_A6[3] + 0x30) + (int)((ulonglong)uVar8 >> 0x20);
  unaff_A6[0xd] = iVar2 + (int)uVar8;
  *(undefined1 *)((int)unaff_A6 + 0x82) = 0xf;
  *unaff_A6 = 0x6f876;
  bVar4 = *(byte *)((int)unaff_A6 + 0x82) - 1;
  *(byte *)((int)unaff_A6 + 0x82) = bVar4;
  if ((int)((uint)bVar4 << 0x18) < 0) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe000;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe000;
  }
  sVar6 = candidate_49e8();
  if (sVar6 == 0) {
    unaff_A6[0xc] = ((int)-unaff_A6[0xc] >> 5) + unaff_A6[0xc];
    uVar5 = -0x100000 - unaff_A6[0xd] >> 5;
    uVar3 = unaff_A6[0xd];
    uVar7 = CARRY4(uVar5,uVar3);
    unaff_A6[0xd] = uVar5 + uVar3;
    func_0x000143e4();
    if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) &&
       ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0)) {
      candidate_1945c();
      if (!(bool)uVar7) {
        candidate_427a();
        return;
      }
      goto candidate_4188;
    }
  }
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_4256();
  *unaff_A6 = 0x6f926;
  uVar7 = 0;
  candidate_1436a();
  candidate_427a();
  if (!(bool)uVar7) {
    return;
  }
candidate_4188:
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
