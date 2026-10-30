/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x088ad2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_88ad2(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  byte bVar4;
  short sVar5;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar6;

  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  uVar6 = 0;
  candidate_146a8();
  candidate_8919c();
  candidate_893d0();
  if ((bool)uVar6) {
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0xaa) = 0;
    *unaff_A6 = 0x88b02;
    uVar6 = 0;
    candidate_4c24();
    candidate_146a8();
    candidate_427a();
    if ((bool)uVar6) {
      candidate_8919c();
      candidate_893d0();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_88ad2;
      }
      else {
        *unaff_A6 = 0x8806e;
      }
    }
    if (unaff_A6[0x28] != 0) {
      candidate_140c0();
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    return;
  }
  *(undefined1 *)(unaff_A6 + 0x2a) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  uVar6 = 0;
  candidate_146a8();
  candidate_8919c();
  candidate_89132();
  if (((bool)uVar6) && (*(short *)(unaff_A6 + 0x1f) == *(short *)((int)unaff_A6 + 0x7a))) {
    *(undefined1 *)((int)unaff_A6 + 0xab) = 1;
    *(undefined2 *)(unaff_A6 + 0x21) = 0x1e;
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *(undefined1 *)((int)unaff_A6 + 0xa9) = 0xff;
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0xaa) = 0;
    *unaff_A6 = 0x8833a;
    candidate_892fe();
    if (*(char *)((int)unaff_A6 + 0xab) == '\0') {
      uVar3 = *(ushort *)(unaff_A6 + 0xf);
      if (((short)uVar3 < 0x30) || (uVar6 = uVar3 < 0x120, 0x120 < (short)uVar3)) goto LAB_00088372;
    }
    else {
      if (*(char *)((int)unaff_A6 + 0xab) != '\x01') {
LAB_00088372:
        candidate_892bc();
        if (unaff_A6[0x28] != 0) {
          func_0x000140c4();
        }
        candidate_89152();
        return;
      }
      sVar5 = *(short *)(unaff_A6 + 0x21);
      uVar6 = sVar5 == 0;
      sVar5 = sVar5 + -1;
      *(short *)(unaff_A6 + 0x21) = sVar5;
      if (sVar5 != 0) goto LAB_00088372;
    }
    candidate_8919c();
    candidate_893d0();
    if ((bool)uVar6) {
      *unaff_A6 = candidate_88ad2;
    }
    else {
      *unaff_A6 = 0x8806e;
    }
    candidate_892bc();
    uVar6 = 0;
    if (unaff_A6[0x28] != 0) {
      uVar6 = 0;
      func_0x000140c4();
    }
    candidate_89152();
    candidate_1945c();
    if (!(bool)uVar6) {
      return;
    }
    goto candidate_4188;
  }
  candidate_89218();
  if (*(char *)((int)unaff_A6 + 0xa9) == '\0') {
    sVar5 = candidate_260c8();
  }
  else {
    sVar5 = candidate_260c8();
  }
  if ((short)(*(short *)((int)unaff_A6 + 0x92) + *(short *)((int)unaff_A6 + 0x7a)) <
      *(short *)(unaff_A6 + 0xf)) {
    sVar5 = -sVar5;
  }
  *(int *)((int)unaff_A6 + 0x62) = (int)sVar5 << 8;
  unaff_A6[0x27] = (int)sVar5 << 8;
  sVar5 = (short)(char)((ushort)sVar5 >> 8);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    sVar5 = -sVar5;
  }
  if (sVar5 < 0) {
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0xaa) = 0;
  }
  else {
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0xaa) = 0;
  }
  *unaff_A6 = 0x88120;
  *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0x27];
  *(char *)(unaff_A6 + 0x2a) = *(char *)(unaff_A6 + 0x2a) + '\x01';
  bVar4 = *(byte *)(unaff_A6 + 0x2a) & 7;
  *(byte *)(unaff_A6 + 0x2a) = bVar4;
  if (bVar4 == 0) {
    candidate_89904();
  }
  candidate_892fe();
  sVar5 = *(short *)((int)unaff_A6 + 0x7a) + *(short *)((int)unaff_A6 + 0x92);
  if (*(int *)((int)unaff_A6 + 0x62) < 0) {
    if (*(short *)(unaff_A6 + 0xf) <= sVar5) {
LAB_00088160:
      *unaff_A6 = candidate_881e6;
    }
  }
  else if (sVar5 <= *(short *)(unaff_A6 + 0xf)) goto LAB_00088160;
  if (*(int *)((int)unaff_A6 + 0x62) < 0) {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      candidate_49e8();
    }
  }
  else if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    candidate_49e8();
  }
  candidate_892bc();
  uVar6 = 0;
  if (unaff_A6[0x28] != 0) {
    uVar6 = 0;
    func_0x000140c4();
  }
  candidate_89152();
  candidate_1945c();
  if (!(bool)uVar6) {
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
