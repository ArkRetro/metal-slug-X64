/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x096a10.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_96a10(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  short sVar4;
  ushort uVar5;
  undefined2 uVar6;
  short extraout_D1w;
  short extraout_D1w_00;
  ushort extraout_D1w_01;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar7;
  bool bVar8;

  candidate_9f88();
  candidate_9ea8();
  candidate_9ea8();
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0x96418;
  candidate_966f0();
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  sVar4 = candidate_96916();
  if (-1 < (short)(extraout_D1w - sVar4)) {
    candidate_353a4();
  }
  *unaff_A6 = 0x96a60;
  sVar4 = candidate_96916();
  if ((short)(extraout_D1w_00 - sVar4) < 0) {
    unaff_A6[0xc] = 0;
    uVar3 = unaff_A6[0xd];
    uVar7 = 0x4fff < uVar3;
    unaff_A6[0xd] = uVar3 - 0x5000;
    candidate_1ac38();
  }
  else if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    unaff_A6[0xc] = 0;
    uVar3 = unaff_A6[0xd];
    uVar7 = 0x4fff < uVar3;
    unaff_A6[0xd] = uVar3 - 0x5000;
    func_0x000143e4();
  }
  else {
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    uVar7 = 0;
    candidate_1456a();
  }
  candidate_427a();
  candidate_c8bec();
  if (!(bool)uVar7) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      uVar5 = candidate_96916();
      bVar8 = extraout_D1w_01 < uVar5;
      if ((-1 < (short)(extraout_D1w_01 - uVar5)) && (uVar6 = candidate_aa610(), bVar8)) {
        *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar6;
        bVar8 = false;
        uVar6 = candidate_aa760();
        if (bVar8) {
          *(undefined2 *)(unaff_A6 + 0x1c) = uVar6;
          candidate_aa83a();
          *unaff_A6 = candidate_96b32;
        }
      }
    }
    else {
      candidate_97ede();
      if ((bool)uVar7) {
        *unaff_A6 = candidate_9756a;
        return;
      }
    }
    return;
  }
  candidate_4192();
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
