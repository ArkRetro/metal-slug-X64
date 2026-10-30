/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0672d0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 candidate_672d0(void)

{
  int *piVar1;
  byte bVar2;
  byte bVar3;
  int *piVar4;
  int unaff_A3;

  for (piVar4 = (int *)0x10c572; piVar1 = _DAT_0010c582, _DAT_0010c582 <= piVar4;
      piVar4 = piVar4 + -4) {
    _DAT_0010c586 = piVar4;
    if ((((piVar4 != _DAT_0010c582) && ((*(byte *)((int)piVar4 + 0xf) & 1) != 0)) &&
        (*(char *)(piVar4 + 1) != '\0')) && (candidate_6721e(), *(char *)(piVar4 + 1) == '\0')) {
      candidate_6763a();
    }
    unaff_A3 = *piVar4;
    if (*(code **)(unaff_A3 + 10) != (code *)0x0) {
      (**(code **)(unaff_A3 + 10))();
    }
  }
  if ((int *)0x10c581 < _DAT_0010c582) {
    return 0xffffffff;
  }
  _DAT_0010c586 = _DAT_0010c582;
  if (cRam0010c58f == '\x01') {
    if (*(byte *)(_DAT_0010c582 + 1) < *(byte *)(_DAT_0010c582 + 3)) {
      candidate_672a8();
      return 0;
    }
    cRam0010c58f = 0;
    while( true ) {
      *(undefined1 *)(piVar1 + 2) = *(undefined1 *)((int)piVar1 + 7);
      candidate_66a94();
      if (*(int *)(unaff_A3 + 4) != 0) break;
      bVar2 = *(char *)((int)piVar1 + 7) + 1;
      if (*(byte *)((int)piVar1 + 6) <= bVar2) {
        bVar2 = 0;
      }
      *(byte *)((int)piVar1 + 7) = bVar2;
    }
    candidate_66ac2();
    candidate_66d84();
    if (*(int *)(*_DAT_0010c586 + 0x10) == -1) {
      __m68k_trap(0xf);
    }
    *(undefined1 *)(_DAT_0010c586 + 2) = *(undefined1 *)((int)_DAT_0010c586 + 7);
    candidate_66aac();
    *(undefined1 *)(_DAT_0010c586 + 2) = *(undefined1 *)((int)_DAT_0010c586 + 7);
    candidate_66aac();
    return 0;
  }
  if (cRam0010c58f != '\x02') {
    if (cRam0010c58f != '\0') {
      return 0xffffffff;
    }
    if (*(byte *)(_DAT_0010c582 + 3) <= *(byte *)(_DAT_0010c582 + 1)) {
      bVar2 = *(byte *)((int)_DAT_0010c582 + 0xf);
      *(byte *)((int)_DAT_0010c582 + 0xf) = bVar2 & 0xfe;
      if ((bVar2 & 1) != 0) {
        candidate_66d84();
      }
      _DAT_0010c586 = piVar1;
      if (*(code **)(*piVar1 + 6) != (code *)0x0) {
        (**(code **)(*piVar1 + 6))();
      }
      _DAT_0010c586 = piVar1;
      *(undefined1 *)(piVar1 + 2) = *(undefined1 *)((int)piVar1 + 7);
      while (*(char *)((int)piVar1 + 0xe) != -1) {
        *(undefined1 *)((int)piVar1 + 0xe) = 0xff;
        candidate_66aac();
        _DAT_0010c586 = piVar1;
      }
      bVar2 = 0;
      if ((*(byte *)((int)piVar1 + 0xd) & 1) != 0) {
        bVar2 = bRam0010c36c;
      }
      bVar3 = 0;
      if ((*(byte *)((int)piVar1 + 0xd) & 2) != 0) {
        bVar3 = bRam0010c372;
      }
      bRam0010c58e = -((bVar2 & 1) != 0) & 1U | -((bVar3 & 1) != 0) & 2U;
      if (bRam0010c58e == 0) {
        bRam0010c58e = -((bVar2 & 2) != 0) & 1U | -((bVar3 & 2) != 0) & 2U;
        if (bRam0010c58e == 0) {
          bRam0010c58e = -((bVar2 & 4) != 0) & 1U | -((bVar3 & 4) != 0) & 2U;
          if (bRam0010c58e == 0) {
            bRam0010c58e = -((bVar2 & 8) != 0) & 1U | -((bVar3 & 8) != 0) & 2U;
            if (bRam0010c58e == 0) {
              bRam0010c58e = -((bVar2 & 0x20) != 0) & 1U | -((bVar3 & 0x20) != 0) & 2U;
              if (bRam0010c58e == 0) {
                bRam0010c58e = -((bVar2 & 0x10) != 0) & 1U | -((bVar3 & 0x10) != 0) & 2U;
                if (bRam0010c58e == 0) {
                  bRam0010c58e = 0;
                  return 0;
                }
                *(undefined1 *)((int)piVar1 + 0xe) = 3;
              }
              else {
                *(undefined1 *)((int)piVar1 + 0xe) = 4;
              }
            }
            else {
              *(undefined1 *)((int)piVar1 + 0xe) = 2;
            }
          }
          else {
            *(undefined1 *)((int)piVar1 + 0xe) = 1;
          }
        }
        else {
          *(byte *)((int)piVar1 + 0xf) = *(byte *)((int)piVar1 + 0xf) & 0xfd;
          candidate_67176();
        }
      }
      else {
        *(byte *)((int)piVar1 + 0xf) = *(byte *)((int)piVar1 + 0xf) | 2;
        candidate_67176();
      }
      while (*(char *)((int)piVar1 + 0xe) != -1) {
        *(undefined1 *)((int)piVar1 + 0xe) = 0xff;
        candidate_66aac();
        _DAT_0010c586 = piVar1;
      }
      return 0;
    }
    candidate_672a8();
    return 0;
  }
  if (*(char *)(_DAT_0010c582 + 1) != '\0') {
    candidate_6721e();
    return 0;
  }
  func_0x00066dfe();
  piVar1 = _DAT_0010c582;
  piVar4 = _DAT_0010c582 + 4;
  _DAT_0010c582 = piVar4;
  if (&DAT_0010c582 <= piVar4) {
    return 0xffffffff;
  }
  cRam0010c58f = 0;
  _DAT_0010c586 = piVar4;
  *(undefined1 *)(piVar1 + 6) = *(undefined1 *)((int)piVar1 + 0x17);
  candidate_66aac();
  if (piVar4 == _DAT_0010c582) {
    _DAT_0010c586 = piVar4;
    *(undefined1 *)(piVar1 + 6) = *(undefined1 *)((int)piVar1 + 0x17);
    candidate_66aac();
  }
  candidate_6763a();
  return 0;
}
