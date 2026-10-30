/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c3214.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_c3214(void)

{
  code *pcVar1;
  byte bVar2;
  ushort uVar3;
  uint uVar4;
  ushort uVar5;
  undefined2 extraout_D0u;
  int iVar6;
  uint uVar7;
  byte bVar10;
  ushort uVar8;
  short sVar9;
  char cVar11;
  undefined1 extraout_D1b;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  undefined3 uVar12;
  uint unaff_D7;
  int iVar13;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar14;
  bool bVar15;

  bVar15 = false;
  candidate_40bc();
  if (!bVar15) goto LAB_000c271c;
  *(undefined2 *)(unaff_A6 + 0x21) = 0x5a;
  candidate_c0f32();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x24) = 2;
  *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) | 4;
  *unaff_A6 = 0xc162a;
  candidate_aa4c8();
  iVar6 = -0x10000;
  if (-1 < unaff_A6[0x10] + -0x1c00000) {
    iVar6 = -0x10000 - (unaff_A6[0x10] + -0x1c00000 >> 5);
  }
  unaff_A6[0xd] = (iVar6 - unaff_A6[0xd] >> 3) + unaff_A6[0xd];
  iVar6 = candidate_29c6();
  unaff_A6[0xc] = ((iVar6 - unaff_A6[0xc]) + -0x8000 >> 4) + unaff_A6[0xc];
  uVar7 = candidate_c0fea();
  if (((uVar7 & 0x20) == 0) && ((0x1bf < *(ushort *)(unaff_A6 + 0x10) || ((uVar7 & 0x10) == 0)))) {
    candidate_143ee();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      candidate_40bc();
      *(undefined1 *)((int)unaff_A6 + 0x61) = 1;
      goto LAB_000c1f4a;
    }
    *(char *)(unaff_A6 + 0x1e) = -(-2 < *(short *)(unaff_A6 + 0xd));
    goto LAB_000c330e;
  }
  unaff_A6[0xd] = 0x20000;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 2;
  candidate_40bc();
  unaff_A6[0x14] = 0x2d0aa8;
  candidate_c0f32();
  *(undefined1 *)(unaff_A6 + 0x24) = 2;
  *(undefined1 *)(unaff_A6 + 0x1e) = 0;
  unaff_A6[0xc] = 0;
  *unaff_A6 = 0xc170c;
  candidate_aa4c8();
  unaff_A6[0xd] = (-0x160000 - unaff_A6[0xd] >> 5) + unaff_A6[0xd];
  candidate_c109c();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_c1ebe;
  }
LAB_000c32f4:
  candidate_19432();
  *(bool *)((int)unaff_A6 + 0x79) = *(char *)(unaff_A6 + 0x24) == '\x03';
  (*(code *)unaff_A6[0x1f])();
LAB_000c330e:
  if (*(char *)((int)unaff_A6 + 0x5a) == '\x0f') {
    *(undefined1 *)((int)unaff_A6 + 0x9e) = 0xff;
  }
  bVar10 = 0;
  if ((*(char *)((int)unaff_A6 + 0x93) == '\0') || (*(char *)((int)unaff_A6 + 0x93) == '\x01')) {
    bVar10 = -(*(char *)(unaff_A6 + 0x1b) == '\t') | -(*(char *)(unaff_A6 + 0x1b) == '\a');
  }
  *(byte *)((int)unaff_A6 + 0x9d) = bVar10;
  bVar15 = false;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    candidate_aa458();
    if (bVar15) {
      bVar10 = *(byte *)((int)unaff_A6 + 0x9a);
      *(byte *)((int)unaff_A6 + 0x9a) = bVar10 | 0x80;
      if ((bVar10 & 0x80) == 0) {
        *unaff_A6 = candidate_c3078;
      }
    }
    else {
      bVar10 = *(byte *)((int)unaff_A6 + 0x9a);
      *(byte *)((int)unaff_A6 + 0x9a) = bVar10 & 0x7f;
      if ((bVar10 & 0x80) != 0) {
        *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) & 0x7f;
        cVar11 = candidate_aa5ac();
        bVar15 = false;
        if (cVar11 == '\0') {
          candidate_19424();
          if (!bVar15) goto code_r0x000c310e;
        }
        else if (cVar11 != '\x01') {
          if (cVar11 == '\x02') {
            unaff_A6[0xc] = 0;
            unaff_A6[0xd] = 0x20000;
LAB_000c271c:
            unaff_A6[0x14] = 0x2d0aa8;
            *(undefined1 *)((int)unaff_A6 + 0x7a) = 0xff;
            candidate_c0f32();
            *(undefined1 *)(unaff_A6 + 0x24) = 2;
            *(undefined1 *)(unaff_A6 + 0x1e) = 0;
            *unaff_A6 = 0xc2692;
LAB_000c2692:
            candidate_c0fea();
            unaff_A6[0xd] = (-0x160000 - unaff_A6[0xd] >> 5) + unaff_A6[0xd];
            uVar7 = candidate_c0fea();
            iVar13 = 0;
            iVar6 = 5;
            if ((uVar7 & 4) != 0) {
              iVar13 = -0x40000;
              iVar6 = 2;
            }
            if ((uVar7 & 8) != 0) {
              iVar13 = 0x40000;
              iVar6 = 2;
            }
            unaff_D7 = iVar13 - unaff_A6[0xc] >> iVar6;
            unaff_A6[0xc] = unaff_D7 + unaff_A6[0xc];
            candidate_c109c();
            *(char *)(unaff_A6 + 0x1e) = -(-2 < *(short *)(unaff_A6 + 0xd));
            candidate_427a();
            if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
              *unaff_A6 = candidate_c1ebe;
            }
            goto LAB_000c32f4;
          }
          if (cVar11 == '\x03') {
LAB_000c1f4a:
            do {
              if ((*(char *)((int)unaff_A6 + 0x93) == '\x02') &&
                 (0x1f < *(byte *)((int)unaff_A6 + 0x86))) {
                *(undefined1 *)((int)unaff_A6 + 0x86) = 0;
                unaff_A6[0x14] = 0x2d0aa8;
                goto LAB_000c1f24;
              }
              *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) | 0x20;
              *(undefined1 *)((int)unaff_A6 + 0x7b) = 0xff;
              *(undefined1 *)((int)unaff_A6 + 0x7a) = 0xff;
              *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8834;
              *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8834;
              unaff_A6[0x14] = 0x2d0aa8;
              *(undefined1 *)(unaff_A6 + 0x24) = 0;
              candidate_c0f32();
              *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
              *(undefined1 *)((int)unaff_A6 + 0x96) = 0;
              *unaff_A6 = 0xc1fa6;
              while( true ) {
                if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c271c;
                if (*(char *)(unaff_A6 + 0x1b) != '\f') break;
LAB_000c2214:
                *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                unaff_A6[0x14] = 0x2d0ab4;
                candidate_c0f32();
                candidate_c0f16();
                *unaff_A6 = 0xc2236;
                if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c271c;
                uVar7 = candidate_c0fea();
                uVar14 = *(byte *)(unaff_A6 + 0x1b) < 0xc;
                if ((*(byte *)(unaff_A6 + 0x1b) != 0xc) && ((uVar7 & 0x20) != 0)) goto LAB_000c2654;
                if ((uVar7 & 0x10) == 0) {
                  if ((uVar7 & 0x40) == 0) {
                    candidate_14594();
                    candidate_427a();
                    if ((bool)uVar14) {
                      *unaff_A6 = 0xc22a4;
                    }
                    goto LAB_000c32f4;
                  }
                  *(undefined1 *)(unaff_A6 + 0x20) = 0;
                  candidate_c0f32();
                }
                else {
                  *(undefined1 *)(unaff_A6 + 0x20) = 1;
                  if ((*(byte *)((int)unaff_A6 + 0x9a) & 1) == 0) {
                    bVar10 = *(byte *)(unaff_A5 + 0x471e + (int)*(short *)(unaff_A6 + 0x1c));
                    if (0xf < bVar10) {
                      __m68k_trap(0xf);
                    }
                    unaff_D7 = (uint)bVar10 << 2;
                  }
                  else if (*(char *)((int)unaff_A6 + 0x9d) == '\0') {
                    *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) ^ 2;
                  }
                  else {
                    *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) ^ 2;
                  }
                  candidate_c0f32();
                  *(undefined1 *)((int)unaff_A6 + 0x81) = 0xff;
                  cVar11 = *(char *)(unaff_A5 + 0x471e + (int)*(short *)(unaff_A6 + 0x1c));
                  if ((cVar11 == '\x05') || (cVar11 == '\x06')) {
                    *(undefined1 *)((int)unaff_A6 + 0x81) = 4;
                  }
                  else if ((cVar11 == '\n') || (cVar11 == '\v')) {
                    *(undefined1 *)((int)unaff_A6 + 0x81) = 4;
                  }
                }
                *(byte *)((int)unaff_A6 + 0x9b) = *(byte *)((int)unaff_A6 + 0x9b) & 0xef;
                *(byte *)(unaff_A6 + 0x27) = *(byte *)(unaff_A6 + 0x27) & 0xef;
                *(byte *)((int)unaff_A6 + 0x9b) = *(byte *)((int)unaff_A6 + 0x9b) & 0xbf;
                *(byte *)(unaff_A6 + 0x27) = *(byte *)(unaff_A6 + 0x27) & 0xbf;
                candidate_c0f16();
                *(undefined1 *)((int)unaff_A6 + 0x76) = 0;
                *unaff_A6 = 0xc2588;
                if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
                  unaff_A6[0xd] = 0xfff00000;
                  candidate_143ee();
                  unaff_A6[0xd] = 0;
                  goto LAB_000c271c;
                }
                uVar7 = candidate_c0fea();
                if (*(char *)(unaff_A6 + 0x1b) == '\f') {
LAB_000c25b0:
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                  uVar14 = 0;
                  candidate_14594();
                  candidate_427a();
                  if ((bool)uVar14) {
                    if (*(char *)(unaff_A6 + 0x20) == '\0') {
                      *unaff_A6 = candidate_c2294;
                    }
                    else {
                      *unaff_A6 = candidate_c229c;
                    }
                  }
                  if (*(char *)((int)unaff_A6 + 0x76) != '\0') {
                    if ((*(byte *)((int)unaff_A6 + 0x9b) & 0x10) != 0) {
                      *unaff_A6 = 0xc246a;
                    }
                    if ((*(byte *)((int)unaff_A6 + 0x9b) & 0x40) != 0) {
                      *unaff_A6 = 0xc2458;
                    }
                    if ((*(byte *)((int)unaff_A6 + 0x9a) & 8) != 0) {
                      *unaff_A6 = 0xc2338;
                    }
                  }
                  cVar11 = *(char *)((int)unaff_A6 + 0x81);
                  if (-1 < cVar11) {
                    if (cVar11 == '\0') {
                      if ((*(byte *)((int)unaff_A6 + 0x9a) & 8) != 0) {
                        *unaff_A6 = 0xc2338;
                      }
                    }
                    else {
                      *(char *)((int)unaff_A6 + 0x81) = cVar11 + -1;
                    }
                  }
                  goto LAB_000c32f4;
                }
                if ((uVar7 & 2) != 0) {
                  if ((uVar7 & 0x20) != 0) goto LAB_000c2654;
                  goto LAB_000c25b0;
                }
                *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
LAB_000c1f24:
                *(undefined1 *)((int)unaff_A6 + 0x92) = 0;
                candidate_c0f32();
                candidate_c0f16();
                *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) & 0xdf;
                *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                *unaff_A6 = 0xc1fa6;
              }
              uVar7 = candidate_c0fea();
              if ((uVar7 & 4) != 0) goto LAB_000c285c;
              if ((uVar7 & 8) != 0) goto LAB_000c285c;
              if ((uVar7 & 0x20) != 0) {
LAB_000c2654:
                unaff_A6[0xc] = 0;
                candidate_c0f32();
                unaff_A6[0x14] = 0x2d0ac0;
                *(undefined1 *)(unaff_A6 + 0x24) = 2;
LAB_000c266e:
                if (*(char *)(unaff_A6 + 0x1b) == '\r') {
                  unaff_A6[0xd] = unaff_A6[0xd] + 0xa9000;
                }
                else {
                  unaff_A6[0xd] = 0xa9000;
                }
                *unaff_A6 = 0xc2692;
                goto LAB_000c2692;
              }
              if ((uVar7 & 2) != 0) goto LAB_000c2214;
              if (((*(char *)((int)unaff_A6 + 0x9d) != '\0') &&
                  ((*(byte *)((int)unaff_A6 + 0x9a) & 1) != 0)) &&
                 (uVar7 = candidate_c0fea(), (uVar7 & 0x10) != 0)) {
                do {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                  unaff_A6[0x14] = 0x2d0aa8;
                  *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) ^ 2;
                  candidate_c0f32();
                  candidate_c0f16();
                  *(undefined1 *)((int)unaff_A6 + 0x76) = 0;
                  *unaff_A6 = 0xc2198;
                  uVar7 = candidate_c0fea();
                  if ((uVar7 & 0x20) != 0) goto LAB_000c1f4a;
                  if (*(char *)((int)unaff_A6 + 0x76) == '\0') {
                    candidate_ca2ce();
                    *(ushort *)(extraout_A0 + 4) = *(ushort *)(extraout_A0 + 4) | 1;
                    if ((*(byte *)((int)unaff_A6 + 0x9a) & 8) == 0) goto LAB_000c21f2;
                  }
                  else {
                    if ((uVar7 & 2) != 0) goto LAB_000c2214;
                    if (((uVar7 & 0x40) != 0) || ((uVar7 & 0x10) != 0)) goto LAB_000c1f4a;
                    if (((uVar7 & 4) == 0) && ((uVar7 & 8) == 0)) {
LAB_000c21f2:
                      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                      uVar14 = 0;
                      candidate_14594();
                      candidate_427a();
                      if ((bool)uVar14) {
                        *unaff_A6 = 0xc1f4a;
                      }
                      goto LAB_000c32f4;
                    }
                  }
LAB_000c285c:
                  while( true ) {
                    unaff_A6[0x14] = 0x2d0aa8;
                    *(undefined1 *)((int)unaff_A6 + 0x77) = 0;
                    candidate_c0f32();
                    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x1c1;
                    *(undefined1 *)(unaff_A6 + 0x24) = 1;
                    *(undefined2 *)(unaff_A6 + 0x20) = 0x3f;
                    *unaff_A6 = 0xc2888;
                    if (*(char *)(unaff_A6 + 0x1b) == '\r') {
                      *(undefined2 *)((int)unaff_A6 + 0x26) = 0x237;
                    }
                    bVar10 = *(byte *)((int)unaff_A6 + 0x86);
                    *(byte *)((int)unaff_A6 + 0x86) = bVar10 + 1;
                    *(byte *)((int)unaff_A6 + 0x86) =
                         -(0xfe < bVar10) | *(byte *)((int)unaff_A6 + 0x86);
                    *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) >> 1;
                    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c271c;
                    if (*(char *)(unaff_A6 + 0x1b) == '\f') goto LAB_000c2214;
                    bVar10 = candidate_c0fea();
                    uVar12 = (undefined3)(unaff_D7 >> 8);
                    if ((*(byte *)((int)unaff_A6 + 0x9a) & 8) == 0) goto LAB_000c28d8;
                    if (*(char *)(unaff_A6 + 0x26) == '\x02') break;
                    unaff_A6[0x14] = 0x2d0aa8;
                    if (*(char *)(unaff_A6 + 0x26) == '\x01') {
                      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                      candidate_c0f32();
                      *unaff_A6 = 0xc27fa;
                      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c271c;
                      if (*(char *)(unaff_A6 + 0x26) == '\x01') {
                        bVar10 = candidate_c0fea();
                        unaff_D7 = CONCAT31(uVar12,bVar10) & 0xffffff0c;
                        if ((bVar10 & 0xc) == 0) goto LAB_000c1f4a;
                        if ((*(byte *)((int)unaff_A6 + 0x9a) & 8) != 0) {
                          if ((bVar10 & 0x20) != 0) goto LAB_000c2654;
                          if ((bVar10 & 2) != 0) goto LAB_000c2214;
                          unaff_D7 = 0x30000;
                          if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
                            unaff_D7 = 0xfffd0000;
                          }
                          *(uint *)((int)unaff_A6 + 0x62) = unaff_D7;
                          candidate_14594();
                          candidate_427a();
                          goto LAB_000c32f4;
                        }
                      }
                    }
                    else {
                      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
                      if (*(char *)(unaff_A6 + 0x26) != '\x02') {
                        candidate_c0f32();
                        candidate_c0f16();
                        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
                          *(undefined4 *)((int)unaff_A6 + 0x62) = 0x30000;
                        }
                        else {
                          *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffd0000;
                        }
                        *unaff_A6 = 0xc27a8;
                        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_000c271c;
                        uVar14 = 0;
                        if (*(char *)(unaff_A6 + 0x26) == '\0') {
                          uVar7 = candidate_c0fea();
                          if ((uVar7 & 0x20) != 0) goto LAB_000c2642;
                          if ((uVar7 & 2) != 0) goto LAB_000c2214;
                          candidate_14594();
                          candidate_427a();
                          if ((bool)uVar14) {
                            *unaff_A6 = 0xc285c;
                          }
                          goto LAB_000c32f4;
                        }
                      }
                    }
                  }
                  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
LAB_000c28d8:
                  unaff_D7 = CONCAT31(uVar12,bVar10) & 0xffffff0c;
                  if ((bVar10 & 0xc) == 0) {
                    if (*(short *)(unaff_A6 + 0x20) != 0) goto LAB_000c1f4a;
                    *(undefined1 *)((int)unaff_A6 + 0x7b) = 0xff;
                    unaff_A6[0x14] = 0x2d0aa8;
                    if (('\0' < *(char *)((int)unaff_A6 + 0x92)) ||
                       (*(undefined4 *)((int)unaff_A6 + 0x62) = 0,
                       *(char *)(unaff_A6 + 0x26) != '\0')) goto LAB_000c1f4a;
                    goto LAB_000c1f24;
                  }
                  if ((bVar10 & 0x20) != 0) {
LAB_000c2642:
                    candidate_c0f32();
                    *(undefined1 *)(unaff_A6 + 0x24) = 3;
                    goto LAB_000c266e;
                  }
                  if ((bVar10 & 2) != 0) goto LAB_000c2214;
                  if (((*(char *)((int)unaff_A6 + 0x9d) == '\0') ||
                      ((*(byte *)((int)unaff_A6 + 0x9a) & 1) == 0)) || ((bVar10 & 0x10) == 0)) {
                    iVar6 = 0x30000;
                    if (*(char *)((int)unaff_A6 + 0x93) == '\x02') {
                      iVar6 = 0x28000;
                    }
                    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
                      iVar6 = -iVar6;
                    }
                    *(int *)((int)unaff_A6 + 0x62) = iVar6;
                    candidate_14594();
                    candidate_427a();
                    goto LAB_000c32f4;
                  }
                } while( true );
              }
              if ((*(byte *)((int)unaff_A6 + 0x9a) & 0x20) != 0) {
                bVar10 = candidate_c0fea();
                uVar14 = false;
                if ((bVar10 & 0x51) == 0) {
                  candidate_18f24();
                  if ((bool)uVar14) {
                    bVar10 = candidate_18ffe();
                    bVar15 = false;
                    if (((*(byte *)((int)unaff_A6 + 0x5e) & bVar10) != 0) &&
                       (candidate_c0f96(), bVar15)) {
                      *unaff_A6 = candidate_c1e84;
                    }
                  }
                  bVar15 = *(byte *)(unaff_A6 + 0x1b) < 2;
                  if ((*(byte *)(unaff_A6 + 0x1b) == 2) && (candidate_c0f96(), bVar15)) {
                    bVar15 = (*(byte *)((int)unaff_A6 + 0x15) & 1) != 0 &&
                             CONCAT22(extraout_D0u,0x10) != 0;
                    candidate_14850();
                    if (!bVar15) {
                      *unaff_A6 = candidate_c1eaa;
                    }
                  }
                  bVar10 = (*(byte *)((int)unaff_A6 + 0x96) + 1) -
                           (0xfe < *(byte *)((int)unaff_A6 + 0x96));
                  *(byte *)((int)unaff_A6 + 0x96) = bVar10;
                  if ((0xf < bVar10) &&
                     (uVar8 = candidate_29c6(), uVar8 < *(byte *)((int)unaff_A6 + 0x96))) {
                    *(undefined1 *)((int)unaff_A6 + 0x96) = 0;
                    uVar14 = false;
                    candidate_c0f96();
                    if ((bool)uVar14) {
                      uVar8 = candidate_29c6();
                      uVar14 = uVar8 < 0x4000;
                      if ((bool)uVar14) {
                        *unaff_A6 = candidate_c1e54;
                        goto LAB_000c20ec;
                      }
                    }
                    candidate_c0f96();
                    if ((bool)uVar14) {
                      *unaff_A6 = candidate_c1e44;
                    }
                  }
LAB_000c20ec:
                  sVar9 = candidate_191fc();
                  uVar14 = 0;
                  if (sVar9 != 0) {
                    uVar14 = sVar9 == 0;
                    if (sVar9 == 1) {
                      candidate_c0f96();
                      if ((bool)uVar14) {
                        uVar8 = candidate_29c6();
                        uVar14 = uVar8 < 0x100;
                        if ((bool)uVar14) {
                          *unaff_A6 = candidate_c1e64;
                          uVar14 = 0;
                        }
                      }
                    }
                    else {
                      candidate_c0f96();
                      if ((bool)uVar14) {
                        uVar8 = candidate_29c6();
                        uVar14 = uVar8 < 0x100;
                        if ((bool)uVar14) {
                          *unaff_A6 = candidate_c1e74;
                          uVar14 = 0;
                        }
                      }
                    }
                  }
                }
                goto LAB_000c2144;
              }
              bVar10 = candidate_c0fea();
              uVar14 = 0;
              if ((bVar10 & 0x51) == 0) {
LAB_000c2144:
                candidate_14594();
                candidate_427a();
                if ((bool)uVar14) {
                  *unaff_A6 = 0xc1f4a;
                }
                goto LAB_000c32f4;
              }
            } while( true );
          }
          if (cVar11 == '\x05') {
            *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
            goto LAB_000c1f4a;
          }
          if (cVar11 == '\x04') {
            *(undefined2 *)((int)unaff_A6 + 0x66) = 0x87f4;
            *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x87f4;
            candidate_9d1e();
            candidate_c0fb6();
            unaff_A6[0xc] = 0;
            unaff_A6[0xd] = 0xc0000;
            unaff_A6[0x20] = 0x100000;
            *unaff_A6 = 0xc31b8;
            candidate_aa4c8();
            unaff_A6[0x20] = (-0x180000 - unaff_A6[0x20] >> 4) + unaff_A6[0x20];
            unaff_A6[0xd] = ((int)(unaff_A6[0x20] - unaff_A6[0xd]) >> 4) + unaff_A6[0xd];
            candidate_143ee();
            candidate_427a();
            if ((int)unaff_A6[0xd] < -0x3ffff) {
              *unaff_A6 = candidate_c3214;
            }
            if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
              *unaff_A6 = candidate_c3214;
            }
            *(ushort *)(unaff_A6 + 0x21) = *(ushort *)(unaff_A6 + 0x21) | 6;
            goto LAB_000c32f4;
          }
        }
        candidate_c0f32();
        candidate_c0f16();
        unaff_A6[0xc] = 0;
        unaff_A6[0xd] = 0xb7000;
        *unaff_A6 = 0xc3250;
        candidate_aa4c8();
        *(ushort *)(unaff_A6 + 0x21) = *(ushort *)(unaff_A6 + 0x21) | 0x1e;
        candidate_c0fea();
        uVar4 = -0x160000 - unaff_A6[0xd] >> 5;
        uVar7 = unaff_A6[0xd];
        bVar15 = CARRY4(uVar4,uVar7);
        unaff_A6[0xd] = uVar4 + uVar7;
        candidate_19424();
        if (!bVar15) {
          uVar7 = candidate_c0fea();
          iVar13 = 0;
          iVar6 = 5;
          if ((uVar7 & 4) != 0) {
            iVar13 = -0x40000;
            iVar6 = 2;
          }
          if ((uVar7 & 8) != 0) {
            iVar13 = 0x40000;
            iVar6 = 2;
          }
          unaff_D7 = iVar13 - unaff_A6[0xc] >> iVar6;
          unaff_A6[0xc] = unaff_D7 + unaff_A6[0xc];
        }
        candidate_c109c();
        candidate_427a();
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
          *unaff_A6 = candidate_c1ebe;
        }
        goto LAB_000c32f4;
      }
    }
  }
  if (((*(short *)((short)(*(short *)(unaff_A6 + 0x1c) * 2) + 0x10c724) == 0) ||
      ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) &&
     (candidate_c0ee2(), *(char *)((int)unaff_A6 + 0x93) == '\x01')) {
    *(undefined1 *)(unaff_A6 + 0x25) = 0;
  }
  bVar15 = *(byte *)((int)unaff_A6 + 0x93) < 2;
  if (*(byte *)((int)unaff_A6 + 0x93) == 2) {
    candidate_c0bae();
    if (!bVar15) {
      bVar15 = false;
      candidate_ca2f8();
      if (!bVar15) {
        *(undefined2 *)((short)(*(short *)(unaff_A6 + 0x1c) * 2) + 0x10c728) = 0;
        *(undefined1 *)(unaff_A6 + 0x25) = 0;
        goto LAB_000c3402;
      }
    }
    sVar9 = *(short *)((short)(*(short *)(unaff_A6 + 0x1c) * 2) + 0x10c728);
    if ((sVar9 != 0) &&
       (sVar9 = sVar9 + -1, *(short *)((short)(*(short *)(unaff_A6 + 0x1c) * 2) + 0x10c728) = sVar9,
       sVar9 == 0)) {
      *(undefined1 *)(unaff_A6 + 0x25) = 0;
    }
  }
LAB_000c3402:
  if (((*(byte *)((int)unaff_A6 + 0x9a) & 0x80) != 0) || (3 < *(byte *)(unaff_A6 + 0x25)))
  goto LAB_000c35d0;
  bVar10 = *(byte *)((int)unaff_A6 + 0x93);
  bVar2 = BYTE_ARRAY_000c32e4[(short)((ushort)*(byte *)(unaff_A6 + 0x25) + (ushort)bVar10 * 4)];
  if ((char)bVar2 < '\0') goto LAB_000c35d0;
  if (bVar2 == 1) {
    if ((code *)unaff_A6[0x1f] == candidate_c381c) {
      if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) ||
         (uVar7 = candidate_c0fea(), (uVar7 & 2) == 0)) goto LAB_000c35d0;
      *(undefined1 *)((int)unaff_A6 + 0x99) = 6;
      *unaff_A6 = 0xc22a4;
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x99) = 6;
      unaff_A6[0x1f] = candidate_c397e;
    }
  }
  else if (bVar2 == 3) {
    candidate_c0ee2();
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    *(undefined2 *)(unaff_A6 + 0x21) = 0x14;
    *unaff_A6 = candidate_c17a0;
  }
  else if (bVar2 == 2) {
    if (((*(byte *)((int)unaff_A6 + 0x9a) & 0x80) != 0) ||
       ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) goto LAB_000c35d0;
    *unaff_A6 = candidate_c1d70;
  }
  else {
    if ((bVar10 == 2) && ((*(byte *)((int)unaff_A6 + 0x9a) & 0x80) == 0)) {
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
        if ((code *)unaff_A6[0x1f] == candidate_c381c) goto LAB_000c3524;
        *unaff_A6 = candidate_c1d82;
      }
      else {
        *unaff_A6 = candidate_c1d82;
      }
    }
    else {
LAB_000c3524:
      if (bVar10 == 3) {
        *unaff_A6 = candidate_c1740;
      }
      else if ((code *)unaff_A6[0x1f] == candidate_c381c) {
        if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) ||
           (uVar7 = candidate_c0fea(), (uVar7 & 2) == 0)) goto LAB_000c35d0;
        *unaff_A6 = candidate_c2280;
      }
      else {
        unaff_A6[0x1f] = candidate_c393c;
      }
    }
    *(undefined1 *)(*(short *)(unaff_A6 + 0x1c) + 0x10c71a) = 0;
  }
  if ((*(char *)((int)unaff_A6 + 0x93) == '\x01') && (*(char *)(unaff_A6 + 0x25) != '\x01')) {
    candidate_411c();
    *(undefined2 *)(extraout_A0_00 + 0x6e) = *(undefined2 *)((int)unaff_A6 + 0x6e);
    candidate_1ac94();
    candidate_1acf2();
  }
  *(undefined1 *)((int)unaff_A6 + 0x93) = *(undefined1 *)(unaff_A6 + 0x25);
  *(undefined1 *)(unaff_A6 + 0x25) = 0xff;
LAB_000c35d0:
  if (*(char *)((int)unaff_A6 + 0x99) != '\0') {
    *(char *)((int)unaff_A6 + 0x99) = *(char *)((int)unaff_A6 + 0x99) + -1;
    candidate_9d1e();
  }
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
  if (*(short *)(unaff_A6 + 0x21) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    sVar9 = *(short *)(unaff_A6 + 0x21) + -1;
    *(short *)(unaff_A6 + 0x21) = sVar9;
    if (sVar9 == 0) {
      *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) & 0xfb;
    }
    else if ((((*(byte *)((int)unaff_A6 + 0x9a) & 4) != 0) && (0xf < *(ushort *)(unaff_A6 + 0x21)))
            && ((*(ushort *)(unaff_A6 + 0x21) & 1) != 0)) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    }
  }
  if ((DAT_0010f000 & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  bVar15 = false;
  candidate_ca2f8();
  if (bVar15) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x40) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  }
  cVar11 = candidate_4c24();
  if (cVar11 == '\x10') {
    if (*(char *)((int)unaff_A6 + 0x93) == '\x03') {
      *unaff_A6 = 0xc2956;
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
      *(undefined1 *)(unaff_A6 + 0x25) = 3;
      candidate_c0ee2();
    }
  }
  else if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = 0xc2956;
  }
  if (*(short *)(unaff_A6 + 0x10) < *(short *)(unaff_A5 + 0x472c)) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
    *unaff_A6 = candidate_c2d6a;
  }
  if (*(char *)((int)unaff_A6 + 0x9e) != '\0') {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 6;
    *unaff_A6 = 0xc2956;
  }
  *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) & 0xfe;
  cVar11 = candidate_49e8();
  if ((cVar11 == '\x05') || (cVar11 == '\x04')) {
    *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) | 1;
  }
  *(undefined1 *)(unaff_A6 + 0x27) = 0xff;
  return;
code_r0x000c310e:
  *(undefined1 *)((int)unaff_A6 + 0x5a) = extraout_D1b;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  *(byte *)((int)unaff_A6 + 0x9a) = *(byte *)((int)unaff_A6 + 0x9a) & 0x7f;
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  candidate_120f8();
  unaff_A6[0x14] = 0;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ec;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ec;
  if (*(char *)((int)unaff_A6 + 0x93) == '\x02') {
    candidate_c0fb6();
    candidate_c0f16();
    *unaff_A6 = 0xc2a48;
    unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 5);
    uVar7 = unaff_A6[0xd];
    uVar14 = uVar7 < (uint)((int)unaff_A6[0xd] >> 5);
    unaff_A6[0xd] = uVar7 - ((int)unaff_A6[0xd] >> 5);
    candidate_143ee();
    candidate_427a();
    if ((bool)uVar14) goto LAB_000c2f90;
    goto LAB_000c2f4a;
  }
  if (*(char *)((int)unaff_A6 + 0x93) == '\x03') {
LAB_000c2c40:
    candidate_9f88();
    candidate_9ea8();
    candidate_9ea8();
LAB_000c2cb0:
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    candidate_c0f32();
    candidate_c0f16();
    *(undefined2 *)(unaff_A6 + 0x20) = 0x80;
    goto LAB_000c2e44;
  }
  switch(*(undefined1 *)((int)unaff_A6 + 0x5a)) {
  case 0:
    break;
  case 1:
    break;
  case 2:
    candidate_9f88();
    candidate_9ea8();
    candidate_40f38();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      unaff_A6[0xc] = 0;
      unaff_A6[0xd] = 0;
    }
    candidate_c0f32();
    candidate_c0f16();
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    }
    *(undefined2 *)(unaff_A6 + 0x20) = 0x100;
    *(undefined1 *)(unaff_A6 + 0x1e) = 0xff;
    goto LAB_000c2e84;
  case 3:
    break;
  case 4:
    break;
  case 5:
    break;
  case 6:
    break;
  case 7:
    candidate_9f88();
    candidate_9ea8();
    candidate_9ea8();
    goto LAB_000c2cb0;
  case 8:
    candidate_9f88();
    if ((&DAT_0010c30c)[*(short *)(unaff_A6 + 0x1c)] == '\0') {
      candidate_9ea8();
    }
    else {
      candidate_9ea8();
    }
    candidate_9ea8();
    candidate_9ea8();
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    candidate_c0f32();
    candidate_c0f16();
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
      unaff_A6[0xc] = 0xfffe0000;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
      unaff_A6[0xc] = 0x20000;
    }
    unaff_A6[0xd] = 0xa9000;
    *(undefined1 *)(unaff_A6 + 0x1e) = 0xff;
    *unaff_A6 = candidate_c2b8a;
    unaff_A6[0xd] = (-0x160000 - unaff_A6[0xd] >> 6) + unaff_A6[0xd];
    uVar7 = unaff_A6[0xc];
    uVar14 = uVar7 < (uint)((int)unaff_A6[0xc] >> 6);
    unaff_A6[0xc] = uVar7 - ((int)unaff_A6[0xc] >> 6);
    candidate_143ee();
    candidate_427a();
    if ((bool)uVar14) {
      *unaff_A6 = candidate_c2f06;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *(undefined1 *)(unaff_A6 + 0x1e) = 0;
    }
    goto LAB_000c2f4a;
  case 9:
    break;
  case 10:
    break;
  case 0xb:
    break;
  case 0xc:
    break;
  case 0xd:
    break;
  case 0xe:
    break;
  case 0xf:
    break;
  case 0x10:
    goto LAB_000c2c40;
  case 0x11:
    break;
  case 0x12:
    candidate_40f38();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) break;
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    }
    candidate_c0f32();
    candidate_c0f16();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *unaff_A6 = candidate_c2ab2;
    uVar14 = 0;
    candidate_14594();
    candidate_427a();
    if ((bool)uVar14) {
      *unaff_A6 = candidate_c2f06;
    }
    goto LAB_000c2f4a;
  case 0x13:
  }
  candidate_9f88();
  if ((&DAT_0010c30c)[*(short *)(unaff_A6 + 0x1c)] == '\0') {
    candidate_9ea8();
  }
  else {
    candidate_9ea8();
  }
  candidate_c0f32();
  candidate_c0f16();
  *(undefined2 *)(unaff_A6 + 0x20) = 0x20;
LAB_000c2e44:
  unaff_A6[0xd] = 0xa9000;
  if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    unaff_A6[0xc] = 0xfffe0000;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    unaff_A6[0xc] = 0x20000;
  }
  candidate_c4c28();
  *(undefined1 *)(unaff_A6 + 0x1e) = 0xff;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
LAB_000c2e84:
  *unaff_A6 = 0xc2e8a;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    unaff_A6[0xd] = (-0x160000 - unaff_A6[0xd] >> 5) + unaff_A6[0xd];
    unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 5);
    candidate_143ee();
  }
  else {
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    candidate_14594();
  }
  uVar3 = (short)(*(short *)(unaff_A6 + 0x20) - *(short *)((int)unaff_A6 + 0x26)) >> 4;
  uVar8 = *(ushort *)((int)unaff_A6 + 0x26);
  bVar15 = CARRY2(uVar3,uVar8);
  *(ushort *)((int)unaff_A6 + 0x26) = uVar3 + uVar8;
  candidate_427a();
  if (bVar15) {
    *unaff_A6 = candidate_c2f06;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    unaff_A6[0xc] = 0;
    *(undefined2 *)(unaff_A6 + 0x20) = 0x100;
    *(undefined1 *)(unaff_A6 + 0x1e) = 0;
  }
LAB_000c2f4a:
  *(undefined1 *)(unaff_A5 + 0x430f) = 0xff;
  candidate_c8d4e();
  if ((short)unaff_D7 == 0) {
    *(undefined1 *)((int)*(short *)(unaff_A6 + 0x1c) + unaff_A5 + 0x430a) = 0xff;
  }
  candidate_aa4c8();
  if ((*(ushort *)(unaff_A6 + 0xf) < 0x140) &&
     ((ushort)(*(short *)(unaff_A6 + 0x10) - 0xc0U) < 0x200)) {
    return;
  }
LAB_000c2f90:
  *unaff_A6 = 0xc2f96;
  cVar11 = candidate_19028();
  if (cVar11 == '\0') {
    return;
  }
  sVar9 = *(short *)(unaff_A6 + 0x1c);
  *(undefined1 *)(sVar9 + 0x10c718) = 0;
  *(undefined1 *)(sVar9 + 0x10c71e) = 0;
  *(undefined1 *)(sVar9 + 0x10c720) = 0;
  *(undefined2 *)((short)(sVar9 * 2) + 0x10c724) = 0;
  if (DAT_0010c2e7 == '\x01') {
    *(undefined2 *)(unaff_A6 + 0x20) = 0x1e;
    *unaff_A6 = 0xc2fe0;
    sVar9 = *(short *)(unaff_A6 + 0x20) + -1;
    *(short *)(unaff_A6 + 0x20) = sVar9;
    if (sVar9 != 0) {
      return;
    }
  }
  uVar8 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
  if (0xfc10 < uVar8) {
    uVar8 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar5 = uRam002fffe8;
    uVar3 = ~uVar8 & 0xfff;
    uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7)) !=
                      0));
    uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar8 ^ uVar5) & 1) != 0) {
      *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
      _DAT_002ffff0 = 1;
      do {
      } while (DAT_002fffff != '\x01');
      uRam00400000 = 0;
      uRam00400002 = *(undefined2 *)((short)(sRam002112e2 * 2) + 0x224000);
      uRam00400004 = *(undefined2 *)((short)(sRam002112e4 * 2) + 0x224000);
      uRam00400006 = *(undefined2 *)((short)(sRam002112e6 * 2) + 0x224000);
      uRam00400008 = *(undefined2 *)((short)(sRam002112e8 * 2) + 0x224000);
      uRam0040000a = *(undefined2 *)((short)(sRam002112ea * 2) + 0x224000);
      uRam0040000c = *(undefined2 *)((short)(sRam002112ec * 2) + 0x224000);
      uRam0040000e = *(undefined2 *)((short)(sRam002112ee * 2) + 0x224000);
      uRam00400010 = *(undefined2 *)((short)(sRam002112f0 * 2) + 0x224000);
      uRam00400012 = *(undefined2 *)((short)(sRam002112f2 * 2) + 0x224000);
      uRam00400014 = *(undefined2 *)((short)(sRam002112f4 * 2) + 0x224000);
      uRam00400016 = *(undefined2 *)((short)(sRam002112f6 * 2) + 0x224000);
      uRam00400018 = *(undefined2 *)((short)(sRam002112f8 * 2) + 0x224000);
      uRam0040001a = *(undefined2 *)((short)(sRam002112fa * 2) + 0x224000);
      uRam0040001c = *(undefined2 *)((short)(sRam002112fc * 2) + 0x224000);
      uRam0040001e = *(undefined2 *)((short)(sRam002112fe * 2) + 0x224000);
      _DAT_00401ffe = 0;
      func_0x0003cf12();
      candidate_3e2a8();
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
  }
  candidate_4126();
  iVar6 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar6 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
