/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x022872.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_22872(void)

{
  char cVar1;
  short sVar3;
  undefined2 uVar4;
  uint uVar2;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_1abc8();
  if ((*(byte *)((int)unaff_A6 + 0x8d) & 8) != 0) {
    *(undefined1 *)(unaff_A6 + 0x23) = 2;
    if (0x3f < *(short *)(unaff_A6 + 0xf)) {
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) & 0xf7;
    }
    return;
  }
  candidate_24a30();
  if (*(char *)((int)unaff_A6 + 0x8e) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x8b);
    if ((cVar1 == '\0') && ((*(byte *)((int)unaff_A6 + 0x8f) & 1) != 0)) {
      *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    }
    if ((cVar1 == '\x01') && ((*(byte *)((int)unaff_A6 + 0x8f) & 2) != 0)) {
      *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    }
    if ((cVar1 == '\x02') && ((*(byte *)((int)unaff_A6 + 0x8f) & 4) != 0)) {
      *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    }
    if ((cVar1 == '\x03') && ((*(byte *)((int)unaff_A6 + 0x8f) & 8) != 0)) {
      *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    }
    if ((cVar1 == '\x04') && ((*(byte *)((int)unaff_A6 + 0x8f) & 0x10) != 0)) {
      *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    }
    if ((cVar1 == '\x05') && ((*(byte *)((int)unaff_A6 + 0x8f) & 0x20) != 0)) {
      *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    }
    if ((cVar1 == '\x06') && ((*(byte *)((int)unaff_A6 + 0x8f) & 0x40) != 0)) {
      *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    }
    goto LAB_00022aa2;
  }
  bVar5 = false;
  if (*(short *)((int)unaff_A6 + 0x92) != 0) {
    *(short *)((int)unaff_A6 + 0x92) = *(short *)((int)unaff_A6 + 0x92) + -1;
    goto LAB_00022aa2;
  }
  sVar3 = candidate_1b04a();
  if (bVar5) goto LAB_00022aa2;
  *(undefined4 *)((int)unaff_A6 + 0x72) = extraout_A0;
  if (*(char *)((int)unaff_A6 + 0x8a) == '\0') {
    uVar4 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x92) = uVar4;
    sVar3 = candidate_260c8();
  }
  if (*(char *)((int)unaff_A6 + 0x8a) == '\x01') {
    uVar4 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x92) = uVar4;
    sVar3 = candidate_260c8();
  }
  if (*(char *)((int)unaff_A6 + 0x8a) == '\x02') {
    uVar4 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x92) = uVar4;
    sVar3 = 1;
  }
  if ((*(char *)((int)unaff_A6 + 0x8f) == '\x7f') ||
     (((*(short *)(unaff_A6 + 0xf) < 0x101 && (sVar3 != 0)) &&
      (*(char *)((int)unaff_A6 + 0x8b) != '\b')))) {
LAB_0002294a:
    uVar2 = 7;
  }
  else if (*(char *)((int)unaff_A6 + 0x8b) == '\a') {
    uVar2 = 8;
  }
  else {
    uVar2 = candidate_29f2();
    if (((*(byte *)((int)unaff_A6 + 0x8f) & '\x01' << (uVar2 & 7)) != 0) ||
       ((short)(*(short *)((short)((short)uVar2 << 2) + 0xe056e) + *(short *)(unaff_A6 + 0xf)) <
        0x20)) {
      sVar3 = 6;
      uVar2 = 0;
      do {
        if (((*(byte *)((int)unaff_A6 + 0x8f) & '\x01' << (uVar2 & 7)) == 0) &&
           (0x1f < (short)(*(short *)((short)((short)uVar2 << 2) + 0xe056e) +
                          *(short *)(unaff_A6 + 0xf)))) goto LAB_000229c4;
        uVar2 = (uint)(ushort)((short)uVar2 + 1);
        sVar3 = sVar3 + -1;
      } while (sVar3 != -1);
      goto LAB_0002294a;
    }
  }
LAB_000229c4:
  *(char *)((int)unaff_A6 + 0x8b) = (char)uVar2;
  *(undefined1 *)((int)unaff_A6 + 0x8e) = 0xff;
LAB_00022aa2:
  if (*(char *)((int)unaff_A6 + 0x8a) == '\x02') {
    if (*(short *)(unaff_A6 + 0xf) < 0xa1) {
      if (*(short *)(unaff_A6 + 0xf) < 0x70) {
        if (*(short *)(unaff_A6 + 0xf) < 0x40) {
          *(undefined1 *)(unaff_A6 + 0x23) = 2;
        }
        else {
          *(undefined1 *)(unaff_A6 + 0x23) = 1;
        }
      }
      else {
        *(undefined1 *)(unaff_A6 + 0x23) = 0;
      }
    }
    else {
      *(undefined1 *)(unaff_A6 + 0x23) = 3;
    }
  }
  else {
    if (*(short *)(unaff_A6 + 0xf) < 0x91) {
      *(undefined1 *)(unaff_A6 + 0x23) = 1;
    }
    if (*(short *)(unaff_A6 + 0xf) < 0x41) {
      *(undefined1 *)(unaff_A6 + 0x23) = 2;
    }
    if (0xdf < *(short *)(unaff_A6 + 0xf)) {
      *(undefined1 *)(unaff_A6 + 0x23) = 0;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_22b34;
  }
  return;
}
