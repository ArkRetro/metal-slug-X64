/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x096ba6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_96ba6(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  ushort uVar4;
  undefined2 uVar5;
  short sVar6;
  ushort extraout_D1w;
  short extraout_D1w_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar7;
  undefined1 uVar8;

  *(undefined1 *)((int)unaff_A6 + 0x92) = 1;
  candidate_1b1dc();
  candidate_966f0();
  *unaff_A6 = 0x96bbe;
  sVar6 = candidate_96916();
  if ((short)(extraout_D1w_00 - sVar6) < 0) {
    unaff_A6[0xc] = 0;
    uVar3 = unaff_A6[0xd];
    uVar8 = 0x4fff < uVar3;
    unaff_A6[0xd] = uVar3 - 0x5000;
    candidate_1ac38();
  }
  else if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    unaff_A6[0xc] = 0;
    uVar3 = unaff_A6[0xd];
    uVar8 = 0x4fff < uVar3;
    unaff_A6[0xd] = uVar3 - 0x5000;
    candidate_143ee();
  }
  else {
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    uVar8 = 0;
    candidate_14594();
  }
  candidate_1ad8c();
  candidate_427a();
  if ((bool)uVar8) {
    *unaff_A6 = candidate_96a10;
    uVar8 = 0;
  }
  candidate_c8bec();
  if (!(bool)uVar8) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      uVar4 = candidate_96916();
      bVar7 = extraout_D1w < uVar4;
      if ((-1 < (short)(extraout_D1w - uVar4)) && (uVar5 = candidate_aa610(), bVar7)) {
        *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar5;
        bVar7 = false;
        uVar5 = candidate_aa760();
        if (bVar7) {
          *(undefined2 *)(unaff_A6 + 0x1c) = uVar5;
          candidate_aa83a();
          *unaff_A6 = candidate_96b32;
        }
      }
    }
    else {
      candidate_97ede();
      if ((bool)uVar8) {
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
