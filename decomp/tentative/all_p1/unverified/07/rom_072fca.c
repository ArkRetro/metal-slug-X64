/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x072fca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_72fca(void)

{
  byte bVar1;
  undefined2 uVar3;
  uint uVar2;
  short sVar4;
  char cVar5;
  undefined1 extraout_D1b;
  int iVar6;
  int iVar7;
  undefined4 *unaff_A6;
  undefined1 uVar8;
  bool bVar9;
  undefined8 uVar10;

  *(byte *)((int)unaff_A6 + 0x95) = *(byte *)((int)unaff_A6 + 0x95) & 0xef;
  *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xef;
  *(byte *)((int)unaff_A6 + 0x95) = *(byte *)((int)unaff_A6 + 0x95) & 0xdf;
  *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xdf;
  *(undefined1 *)((int)unaff_A6 + 0x97) = 0;
LAB_00072fe8:
  *(undefined1 *)((int)unaff_A6 + 0x8a) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  if (*(char *)((int)unaff_A6 + 0x91) == '\0') {
    candidate_74c00();
    *unaff_A6 = 0x7300a;
    if (*(char *)((int)unaff_A6 + 0x91) != '\0') goto LAB_00072fe8;
  }
  else {
    candidate_74c00();
    *unaff_A6 = 0x7304c;
    if (*(char *)((int)unaff_A6 + 0x91) == '\0') goto LAB_00072fe8;
  }
  while( true ) {
    while( true ) {
      while( true ) {
        uVar2 = candidate_74ce4();
        if ((uVar2 & 0x80) != 0) goto LAB_000736a2;
        if (((uVar2 & 2) != 0) && ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0))
        goto LAB_0007319e;
        if (((uVar2 & 2) == 0) &&
           ((*(char *)((int)unaff_A6 + 0x97) != '\0' &&
            ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)))) goto LAB_00073124;
        if ((uVar2 & 0x20) != 0) goto LAB_000733b6;
        if ((uVar2 & 0xc) != 0) goto LAB_000732ec;
        if ((((*(byte *)((int)unaff_A6 + 0x8e) & 0x20) == 0) && ((uVar2 & 0x10) != 0)) &&
           ((*(byte *)((int)unaff_A6 + 0x8e) & 4) != 0)) goto LAB_0007363c;
        candidate_73cc0();
        cVar5 = *(char *)(unaff_A6 + 0x23);
        if (cVar5 != '\0') break;
        candidate_74c00();
        *unaff_A6 = 0x73072;
      }
      if (cVar5 != '\x02') break;
      candidate_74c00();
      *unaff_A6 = 0x73024;
    }
    if (cVar5 != '\x03') break;
    candidate_74c00();
    *unaff_A6 = 0x73038;
  }
  if ((cVar5 == '\x04') || (cVar5 == '\x05')) goto LAB_00072fe8;
  candidate_74ba4();
  candidate_74c3c();
LAB_00073b8a:
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *(undefined1 *)(unaff_A6 + 0x25) = 1;
    *(undefined1 *)((int)unaff_A6 + 0x9d) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x9a) = 0x40;
  }
  *(byte *)((int)unaff_A6 + 0x8e) = *(byte *)((int)unaff_A6 + 0x8e) & 0xfb;
  cVar5 = candidate_49e8();
  if ((cVar5 == '\x05') || (cVar5 == '\x04')) {
    *(byte *)((int)unaff_A6 + 0x8e) = *(byte *)((int)unaff_A6 + 0x8e) | 4;
  }
  if (*(short *)((int)unaff_A6 + 0x82) != 0) {
    *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  bVar1 = *(byte *)((int)unaff_A6 + 0x5f);
  *(byte *)((int)unaff_A6 + 0x5f) = bVar1 & 0xbf;
  if ((bVar1 & 0x40) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if (*(int *)((int)unaff_A6 + 0x6e) == 0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
    if ((*(ushort *)((int)unaff_A6 + 0x82) & 1) != 0) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    }
  }
  else {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    candidate_74be0();
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  if ((DAT_0010f000 & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  bVar9 = false;
  candidate_aa760();
  if (bVar9) {
    cVar5 = candidate_aaa88();
    if (cVar5 == '\v') {
      *(undefined1 *)(unaff_A6 + 0x21) = extraout_D1b;
      *(undefined1 *)((int)unaff_A6 + 0x86) = 8;
    }
  }
  else {
    *unaff_A6 = candidate_72e86;
  }
  bVar9 = false;
  candidate_aa9e4();
  if (bVar9) {
    *(undefined2 *)((int)unaff_A6 + 0x82) = 0x1e;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_7387a;
  }
LAB_00073ca0:
  *(byte *)((int)unaff_A6 + 0x8e) = *(byte *)((int)unaff_A6 + 0x8e) & 0xbf;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(undefined1 *)((int)unaff_A6 + 0x96) = 0xff;
  if (*(char *)(unaff_A6 + 0x27) != '\0') {
    *(char *)(unaff_A6 + 0x27) = *(char *)(unaff_A6 + 0x27) + -1;
  }
  return;
LAB_000732ec:
  if ((*(byte *)((int)unaff_A6 + 0x8e) & 2) == 0) {
    candidate_74c00();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffe4000;
  }
  else {
    candidate_74c00();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0x1c000;
  }
  *(undefined1 *)((int)unaff_A6 + 0x8a) = 1;
  *unaff_A6 = 0x7332a;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_00073448;
  uVar2 = candidate_74ce4();
  if ((uVar2 & 0x80) != 0) goto LAB_000736a2;
  if (((uVar2 & 2) != 0) && ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) goto LAB_0007319e;
  if ((uVar2 & 0x20) != 0) goto LAB_000733b6;
  if ((((*(byte *)((int)unaff_A6 + 0x8e) & 0x20) == 0) && ((uVar2 & 0x10) != 0)) &&
     ((*(byte *)((int)unaff_A6 + 0x8e) & 4) != 0)) goto LAB_0007363c;
  if ((uVar2 & 8) != 0) goto code_r0x0007337e;
  if ((uVar2 & 4) != 0) {
    if (*(short *)((int)unaff_A6 + 0x62) < 1) goto LAB_000733a2;
    goto LAB_000732ec;
  }
  goto LAB_00072fe8;
LAB_0007319e:
  do {
    candidate_74c00();
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 2;
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 1;
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *unaff_A6 = 0x731be;
LAB_000731be:
    do {
      while( true ) {
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_00073448;
        uVar2 = candidate_74ce4();
        if ((uVar2 & 0x80) != 0) goto LAB_000736a2;
        if ((((uVar2 & 2) == 0) && (*(char *)((int)unaff_A6 + 0x97) != '\0')) &&
           ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) goto LAB_00073124;
        if ((((*(byte *)((int)unaff_A6 + 0x8e) & 0x20) != 0) || ((uVar2 & 0x10) == 0)) ||
           ((*(byte *)((int)unaff_A6 + 0x8e) & 4) == 0)) break;
        candidate_74c00();
        *(byte *)((int)unaff_A6 + 0x95) = *(byte *)((int)unaff_A6 + 0x95) & 0xef;
        *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xef;
        *(undefined1 *)((int)unaff_A6 + 0x8a) = 2;
        *(undefined1 *)((int)unaff_A6 + 0x8d) = 2;
        *(undefined1 *)(unaff_A6 + 0x27) = *(undefined1 *)(*(short *)(unaff_A6 + 0x20) + 0x732ce);
        *unaff_A6 = 0x732c2;
        if (*(char *)(unaff_A6 + 0x27) == '\0') goto LAB_0007319e;
      }
      if ((uVar2 & 0x20) != 0) {
        *(undefined2 *)(unaff_A6 + 0x20) = 0;
        candidate_74c00();
        candidate_73cd8();
        *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
        candidate_74b10();
        *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
        *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
        *(byte *)((int)unaff_A6 + 0x8e) = *(byte *)((int)unaff_A6 + 0x8e) & 0xfe;
        candidate_353a4();
        candidate_1b1dc();
        unaff_A6[0x14] = 0;
        *unaff_A6 = 0x72ed6;
        uVar8 = 0;
        candidate_74b74();
        candidate_74c3c();
        candidate_1945c();
        if ((bool)uVar8) {
          *unaff_A6 = 0x73ab0;
          uVar8 = 0;
        }
        uVar3 = candidate_aa610();
        if ((bool)uVar8) {
          *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar3;
          bVar9 = false;
          uVar3 = candidate_aa760();
          if (bVar9) {
            *(undefined2 *)(unaff_A6 + 0x20) = uVar3;
            candidate_aa83a();
            *unaff_A6 = candidate_72f3e;
          }
        }
        goto LAB_00073ca0;
      }
      *(byte *)((int)unaff_A6 + 0x8e) = *(byte *)((int)unaff_A6 + 0x8e) | 0x40;
      candidate_73cc0();
      if (*(char *)(unaff_A6 + 0x23) != '\x01') {
        if (0x10 < *(byte *)((int)unaff_A6 + 0x97)) {
          if (*(char *)(unaff_A6 + 0x23) == '\0') {
            candidate_74c00();
            *(undefined1 *)((int)unaff_A6 + 0x8d) = 4;
            *unaff_A6 = 0x732e8;
            goto LAB_000731be;
          }
          if (*(char *)(unaff_A6 + 0x23) == '\x05') break;
        }
        candidate_74ba4();
        candidate_74c3c();
        goto LAB_00073b8a;
      }
      candidate_74c00();
      *(byte *)((int)unaff_A6 + 0x95) = *(byte *)((int)unaff_A6 + 0x95) & 0xbf;
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
      *(undefined1 *)((int)unaff_A6 + 0x8a) = 2;
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 3;
      *(undefined1 *)(unaff_A6 + 0x27) = 0xb;
      *unaff_A6 = 0x73284;
    } while (*(char *)(unaff_A6 + 0x27) != '\0');
  } while( true );
LAB_00073124:
  candidate_74c00();
  *(undefined1 *)((int)unaff_A6 + 0x8a) = 2;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0x7313e;
  uVar8 = 0;
  uVar2 = candidate_74ce4();
  if ((uVar2 & 0x80) != 0) goto LAB_000736a2;
  if (((uVar2 & 2) == 0) || ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) goto LAB_0007315c;
  goto LAB_0007319e;
LAB_0007315c:
  if ((uVar2 & 0x20) == 0) {
    if ((((*(byte *)((int)unaff_A6 + 0x8e) & 0x20) == 0) && ((uVar2 & 0x10) != 0)) &&
       ((*(byte *)((int)unaff_A6 + 0x8e) & 4) != 0)) {
LAB_0007363c:
      candidate_74c00();
      candidate_73cd8();
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x97) = 0;
      *unaff_A6 = 0x7365a;
      uVar8 = 0;
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_00073448;
      uVar2 = candidate_74ce4();
      if ((uVar2 & 0x80) != 0) goto LAB_000736a2;
      candidate_73cc0();
      candidate_74ba4();
      candidate_74c3c();
      if ((bool)uVar8) {
        *unaff_A6 = candidate_72fca;
      }
      if (*(char *)((int)unaff_A6 + 0x8f) != '\0') {
        candidate_49e8();
      }
    }
    else {
      candidate_73cc0();
      candidate_74ba4();
      candidate_74c3c();
      if ((bool)uVar8) {
        *unaff_A6 = 0x72fe2;
      }
    }
  }
  else {
LAB_000733b6:
    candidate_74c00();
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 3;
    *(byte *)((int)unaff_A6 + 0x95) = *(byte *)((int)unaff_A6 + 0x95) & 0xef;
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xef;
    candidate_1b1dc();
    *(undefined1 *)((int)unaff_A6 + 0x97) = 0;
    *unaff_A6 = 0x733e4;
    uVar8 = 0;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
LAB_00073448:
      *(byte *)((int)unaff_A6 + 0x95) = *(byte *)((int)unaff_A6 + 0x95) & 0xdf;
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xdf;
      do {
        *(undefined1 *)((int)unaff_A6 + 0x8a) = 0;
        candidate_74c00();
        *unaff_A6 = 0x7346a;
        while( true ) {
          while( true ) {
            uVar2 = candidate_74ce4();
            if ((uVar2 & 0x80) != 0) goto LAB_000736a2;
            if ((uVar2 >> 0x10 & 0x20) == 0) {
              *(undefined1 *)((int)unaff_A6 + 0x9d) = 0;
              *(byte *)((int)unaff_A6 + 0x95) = *(byte *)((int)unaff_A6 + 0x95) & 0xdf;
              *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xdf;
            }
            if ((((*(byte *)((int)unaff_A6 + 0x95) & 0x20) == 0) ||
                (-1 < *(short *)(unaff_A6 + 0xd))) || (*(char *)(unaff_A6 + 0x25) == '\0')) break;
            *(byte *)((int)unaff_A6 + 0x95) = *(byte *)((int)unaff_A6 + 0x95) & 0xdf;
            *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xdf;
            *(char *)(unaff_A6 + 0x25) = *(char *)(unaff_A6 + 0x25) + -1;
            *(undefined1 *)((int)unaff_A6 + 0x9d) = 8;
            *unaff_A6 = 0x7348a;
          }
          candidate_73cc0();
          if (*(char *)(unaff_A6 + 0x23) == '\x05') break;
          if (*(char *)((int)unaff_A6 + 0x91) != '\x03') {
            if ((*(byte *)((int)unaff_A6 + 0x8e) & 0x80) != 0) {
              candidate_74c00();
            }
LAB_00073524:
            if (*(char *)((int)unaff_A6 + 0x9d) == '\0') {
              unaff_A6[0xd] = unaff_A6[0xd] + -0xa000;
            }
            else {
              *(char *)((int)unaff_A6 + 0x9d) = *(char *)((int)unaff_A6 + 0x9d) + -1;
              unaff_A6[0xd] = (0x60000 - unaff_A6[0xd] >> 1) + unaff_A6[0xd];
            }
            uVar2 = candidate_74ce4();
            if ((uVar2 & 0x80) != 0) goto LAB_000736a2;
            iVar7 = 0;
            iVar6 = 5;
            if ((uVar2 & 4) != 0) {
              iVar7 = -0x40000;
              iVar6 = 2;
            }
            if ((uVar2 & 8) != 0) {
              iVar7 = 0x40000;
              iVar6 = 2;
            }
            unaff_A6[0xc] = (iVar7 - unaff_A6[0xc] >> iVar6) + unaff_A6[0xc];
            candidate_143ee();
            candidate_74c3c();
            if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
              *unaff_A6 = candidate_735ae;
            }
            goto LAB_00073b8a;
          }
          if (*(char *)(unaff_A6 + 0x23) != '\0') goto LAB_00073524;
          candidate_74c00();
          *unaff_A6 = 0x7349e;
        }
      } while( true );
    }
    uVar2 = candidate_74ce4();
    if ((uVar2 & 0x80) != 0) {
LAB_000736a2:
      candidate_aa7fc();
      candidate_74c00();
      candidate_73cd8();
      *(undefined1 *)((int)unaff_A6 + 0x8a) = 4;
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 1;
      candidate_1b1dc();
      *(undefined1 *)((int)unaff_A6 + 0x97) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x9d) = 0;
      *unaff_A6 = 0x736e2;
      uVar8 = false;
      if ((*(short *)(unaff_A6 + 0xd) < 1) && (candidate_1945c(), (bool)uVar8)) {
        *unaff_A6 = 0x73ab0;
        uVar8 = 0;
      }
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
        candidate_74c00();
        candidate_49e8();
        uVar10 = candidate_3b332();
        *(undefined8 *)(unaff_A6 + 0xc) = uVar10;
        *unaff_A6 = 0x73776;
        bVar9 = false;
        if ((*(short *)(unaff_A6 + 0xd) < 1) && (candidate_1945c(), bVar9)) {
          *unaff_A6 = 0x73ab0;
        }
        unaff_A6[0xd] = unaff_A6[0xd] + -0xa000;
        candidate_74b98();
        candidate_74c3c();
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
          *unaff_A6 = candidate_737d0;
        }
        sVar4 = candidate_49e8();
        if (sVar4 != 0) {
          *unaff_A6 = candidate_737d0;
        }
      }
      else {
        candidate_73cc0();
        candidate_1436a();
        candidate_74c3c();
        if ((bool)uVar8) {
          *unaff_A6 = 0x73728;
        }
      }
      goto LAB_00073ca0;
    }
    candidate_73cc0();
    candidate_74bc8();
    candidate_74c3c();
    if ((bool)uVar8) {
      *unaff_A6 = candidate_73418;
    }
  }
  goto LAB_00073b8a;
code_r0x0007337e:
  if (-1 < *(short *)((int)unaff_A6 + 0x62)) {
LAB_000733a2:
    candidate_73cc0();
    candidate_74ba4();
    candidate_74c3c();
    goto LAB_00073b8a;
  }
  goto LAB_000732ec;
}
