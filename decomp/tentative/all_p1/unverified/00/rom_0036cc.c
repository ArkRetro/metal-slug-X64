/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0036cc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint candidate_36cc(void)

{
  uint *puVar1;
  char cVar2;
  byte bVar3;
  uint in_D0;
  uint uVar4;
  undefined1 uVar5;
  int unaff_A5;
  bool bVar6;

  if (*(char *)(unaff_A5 + 0x7dae) == '\x01') {
    bVar3 = *(byte *)(unaff_A5 + 0x7008);
    *(byte *)(unaff_A5 + 0x7008) = bVar3 | 0x80;
    if ((bVar3 & 0x80) == 0) {
      uVar4 = candidate_1004();
      *(undefined1 *)(unaff_A5 + 0x7008) = 0;
      return uVar4;
    }
  }
  else {
    bVar3 = *(char *)(unaff_A5 + 0x7009) + 1;
    in_D0 = (uint)bVar3;
    if (*(char *)(unaff_A5 + 0x7009) != -1) {
      *(byte *)(unaff_A5 + 0x7009) = bVar3;
      DAT_00300001 = bVar3;
    }
    bVar3 = *(byte *)(unaff_A5 + 0x7008);
    *(byte *)(unaff_A5 + 0x7008) = bVar3 | 0x80;
    if ((bVar3 & 0x80) == 0) {
      candidate_272a();
      candidate_8512();
      cVar2 = *(char *)(unaff_A5 + -0x7f74);
      *(char *)(unaff_A5 + -0x7f74) = cVar2 + -1;
      if (cVar2 == '\0') {
        *(undefined1 *)(unaff_A5 + -0x7f74) = 0;
        bVar6 = false;
        uVar5 = candidate_11e48();
        if (bVar6) {
          if ((DAT_0010f001 & 0x10) != 0) {
            DAT_003a0011 = uVar5;
          }
          *(undefined1 *)(unaff_A5 + -0x7f74) = 1;
          *(undefined1 *)(unaff_A5 + -0x7f75) = 0xff;
          candidate_c7ada();
          uVar5 = candidate_11e60();
          puVar1 = *(uint **)(unaff_A5 + -0x6f66);
          if (puVar1 < &DAT_0010109a) {
            uVar4 = *puVar1;
            DAT_00300001 = uVar5;
            *(uint **)(puVar1[0xd] - 0x6f66) = puVar1 + 0x10;
            return uVar4;
          }
          unaff_A5 = 0x108000;
        }
        if ((DAT_0010f001 & 0x10) != 0) {
          DAT_003a0001 = uVar5;
        }
      }
      in_D0 = candidate_121da();
      if ((((DAT_0010c2e7 == '\x01') && (DAT_0010008d != '\0')) && (DAT_0010109f != '\0')) &&
         (_DAT_00100092 == 0)) {
        in_D0 = _DAT_001010a0 + 1;
        if (0x57e3e < in_D0) {
          in_D0 = 359999;
        }
        _DAT_001010a0 = in_D0;
        if (DAT_0010109e != '\0') {
          candidate_3d244();
          in_D0 = candidate_35b2();
        }
      }
    }
  }
  *(undefined1 *)(unaff_A5 + 0x7008) = 0;
  return in_D0;
}
